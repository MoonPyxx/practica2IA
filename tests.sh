#!/bin/bash

# Comprobar que se pasa exactamente un argumento
if [ "$#" -ne 1 ]; then
    echo "Error: Debes proporcionar exactamente un argumento."
    exit 1
fi

# Ejecutar comando basado en el argumento
case $1 in
    11)
        ./practica2 ./mapas/mapa30.map 1 1 9 3 0 7 3 6 3 3
        ;;
    12)
        ./practica2 ./mapas/mapa30.map 1 1 24 12 6 18 13 6 18 15
        ;;
    13)
        ./practica2 ./mapas/mapa30_nuevo.map 1 1 8 9 4 5 5 6 3 3
        ;;
    14)
        ./practica2 mapas/mapa30.map 1 1 25 11 6 5 10 6 3 3
        ;;
    15)
        ./practica2 mapas/mapa30.map 1 1 9 18 4 3 15 1 3 3
        ;;
    16)
        ./practica2 mapas/mapa30.map 1 1 8 13 0 11 18 0 18 12
        ;;
        
    21)
    ./practica2 ./mapas/mapa30.map 1 2 3 13 4 4 4 4 13 13
    ;;
    22)
    ./practica2 ./mapas/mapa50.map 1 2 19 7 6 3 3 3 16 13
    ;;
    23)
    ./practica2 ./mapas/paldea2.map 1 2 82 49 0 73 52 0 25 25
    ;;
    24)
    ./practica2 ./mapas/paldea2.map 1 2 35 79 0 50 63 0 85 16
    ;;
    25)
    ./practica2 ./mapas/scape.map 1 2 9 13 6 6 9 0 9 25
    ;;
    26)
    ./practica2 ./mapas/2ez.map 1 2 25 15 0 4 15 4 5 15
    ;;
    27)
    ./practica2 ./mapas/mapa50.map 1 2 22 14 0 25 7 0 19 14
    ;;
    28)
    ./practica2 ./mapas/ruinados.map 1 2 90 72 6 66 61 5 10 18
    ;;
    29)
    ./practica2 ./mapas/ruinados.map 1 2 90 72 6 93 82 3 10 18  
    ;;
    210)
    ./practica2 mapas/marymonte.map 1 2 8 49 6 3 3 3 9 31
    ;;
    211)
    ./practica2 mapas/marymonte.map 1 2 67 62 6 3 3 3 67 61
    ;;
    212)
    ./practica2 mapas/paldea2.map 1 2 60 94 0 46 43 0 39 3
    ;;

    *)
        echo "Argumento no válido: $1. Por favor, usa un número del 1 al 6."
        exit 2
        ;;
esac
