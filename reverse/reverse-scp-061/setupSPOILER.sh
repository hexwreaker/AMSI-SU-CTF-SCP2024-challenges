#!/bin/bash

# Compilation
gcc -O2 -s -static scp-061.c -o scp-061 -lFLAC -lcrypto -logg -lzstd -lz -lm
# Nettoyage
strip -s ./scp-061
# Création de l'audio contenant le flag
chmod +x ./scp-061
./scp-061 "AMSI{Ess4aye_d4ttr4per_loise4u_p4rl4f3n3tr3}"

mkdir ./public
cp ./scp-061 ./public/
cp ./output.audio ./public/cd-musique-pirate.audio
cp ./rapport-FR92-X-337.md ./public/
