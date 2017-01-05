#!/bin/bash
#Usage cross.sh

if [ $# -ne 1 ]
then
    echo "Usage: ./cross.sh [0: no copy // 1: copy over ssh // 2: copy over bluetooth]"
    exit 1;
fi;

if [ ! -e /usr/bin/docker ]
then
    echo "You need docker"
    exit 1;
fi;

id=$(docker images -q leeproject/container)

if [ -z $id ]
then
    echo "You need to load the image first"
    exit 1
fi;

echo -n "Starting compilation and running (that may take some time).... "


dirFolder=$(mktemp -d)

homeDir=/home/compiler
res=$(docker run -t -v $dirFolder:$homeDir/bin leeproject/container /bin/bash "script.sh" 1>/dev/null)
echo "done"

if [ ! -d 'bin' ]
then
    mkdir bin
fi;

mv $dirFolder/testOsRobot bin/crossCompiled
rmdir $dirFolder
echo "Cross compilation done!"

if [ $1 -eq 0 ]
then
    echo "No copy"
    exit 0;
fi;

if [ $1 -eq 1 ]
then
    addr="10.42.0.103"
else
    addr="10.42.0.99"
fi;

if [ ! -e '/usr/bin/sshpass' ]
then
    echo "You need sshpass to continue."
    echo "sudo apt-get install sshpass"
    exit;
fi;

sshpass -p "maker" | scp bin/crossCompiled robot@$addr:~/osrobot/bin/crossCompiled
echo "Copy completed on the robot"
