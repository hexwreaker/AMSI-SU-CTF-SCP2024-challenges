#!/bin/sh

IMGTAG="crypto-sans-effraction"
IMGNAME="crypto-sans-effraction-image"

# Compile "watchdog"
gcc watchdog.c -o watchdog


# Build the docker image
echo "BUILD docker $IMGTAG"
docker build -t $IMGTAG .

# Kill potentiel previous docker
echo "KILL docker $IMGTAG"
docker kill $IMGNAME
docker rm $IMGNAME

# Run the docker image
#   - forward port of the docker 4001 to machine 4001
echo "RUN docker tag:$IMGTAG   name:$IMGNAME"
docker run --name $IMGNAME -p 4001:4001 $IMGTAG



