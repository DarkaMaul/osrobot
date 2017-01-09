#!/bin/bash
#Usage cross.sh

if [ $# -ne 2 ]
then
    echo "Usage: ./cross.sh [0: no copy // 1: copy over usb // 2: copy over bluetooth] [1 git pull || 2 copy local file]"
    exit 1;
fi;

DOCKER=$(which docker)
if [ ! -e $DOCKER ]
then
    echo "You need docker"
    exit 1;
fi;

IMAGE_NAME='leeproject/container'
id=$(docker images -q $IMAGE_NAME)

if [ -z $id ]
then
    echo "You need to load the image first"
    exit 1
fi;

SCRIPT_NAME='script.sh'
CONTAINER_BIN_DIR='/home/compiler/bin'
CONTAINER_COPY_DIR='/home/compiler/copyDirectory'

localBinDir=$(mktemp -d)

echo -n "Starting compilation and running (that may take some time).... "
if [ $2 -eq 2 ]
then
    localCopyDir=$(mktemp -d)
    cp -R src includes Makefile $localCopyDir
    $DOCKER run -t -v $localCopyDir:$CONTAINER_COPY_DIR -v $localBinDir:$CONTAINER_BIN_DIR $id /bin/bash "$SCRIPT_NAME" "1"
    rm -fr $localCopyDir
else
    $DOCKER run -t -v $localBinDir:$CONTAINER_BIN_DIR $id /bin/bash "$SCRIPT_NAME" "0"
fi;

echo "...done"

if [ ! -d 'bin' ]
then
    mkdir bin
fi;

mv $localBinDir/testOsRobot bin/crossCompiled
rmdir $localBinDir

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

sshpass -p "maker" scp bin/crossCompiled robot@$addr:~/osrobot/bin/crossCompiled
echo "Copy completed on the robot"
