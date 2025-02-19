#!/bin/sh

IMGTAG="bienvenue-01-introduction"
IMGNAME="bienvenue-01-introduction-image"

# Compile "terminal"
gcc terminal.c -o terminal


# Build the docker image
echo "BUILD docker $IMGTAG"
docker build -t $IMGTAG .

# Kill potentiel previous docker
echo "KILL docker $IMGTAG"
docker kill $IMGNAME
docker rm $IMGNAME

# Run the docker image
#   - forward port of the docker 4000 to machine 4000
echo "RUN docker tag:$IMGTAG   name:$IMGNAME"
docker run --name $IMGNAME -p 4000:4000 $IMGTAG

