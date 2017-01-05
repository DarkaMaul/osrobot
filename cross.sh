#!/bin/bash
#Usage cross.sh
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
#rmdir $dirFolder
echo "Cross compilation done!"
