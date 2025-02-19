#!/bin/sh

IMGTAG="tadabase-01-pwn"
IMGNAME="tadabase-01-pwn-image"

# Compile "tadabase"
gcc tadabase.c -o tadabase
mv tadabase ./public/disquette_tadabase1.3.0

# Build the docker image
echo "BUILD docker $IMGTAG"
docker build -t $IMGTAG .

# Kill potentiel previous docker
echo "KILL docker $IMGTAG"
docker kill $IMGNAME
docker rm $IMGNAME

# Run the docker image
#   - forward port of the docker 4006 to machine 4006
echo "RUN docker tag:$IMGTAG   name:$IMGNAME"
docker run --cap-drop=ALL --read-only --security-opt no-new-privileges:true --name $IMGNAME -p 4006:4006 $IMGTAG

