## Small motor
Max speed limit : 1560 counts/s

## Big motors
Max speed limit : 1050 counts/s

## Color sensor
Optimal accuracy: ~0.1-4 cm

##Probleme sur le serveur
* ID de l'allié pas bonne (il manque 1)
* Message ball pas implémenté
* Impossible d'envoyer des coordonnées négatives sur le Fake client
* Log ne  marche pas ?

##S'amuser
``#!/bin/bash

i=0
while [ $i -lt 1000 ]; do
    touch a
    git add .
    git commit -m "Test $i"
    git push
    rm -f a
    git commit -m "Remove"
    git push
done;
