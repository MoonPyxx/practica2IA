#!/bin/bash

# Comprobar que se pasa exactamente un argumento
if [ "$#" -ne 1 ]; then
    echo "Error: Debes proporcionar exactamente un argumento."
    exit 1
fi

# Ejecutar comando basado en el argumento
case $1 in
    1)
        ./practica2 ./mapas/mapa30.map 1 1 9 3 0 7 3 6 3 3
        ;;
    2)
        ./practica2 ./mapas/mapa30.map 1 1 24 12 6 18 13 6 18 15
        ;;
    3)
        ./practica2 ./mapas/mapa30_nuevo.map 1 1 8 9 4 5 5 6 3 3
        ;;
    4)
        ./practica2 mapas/mapa30.map 1 1 25 11 6 5 10 6 3 3
        ;;
    5)
        ./practica2 mapas/mapa30.map 1 1 9 18 4 3 15 1 3 3
        ;;
    6)
        ./practica2 mapas/mapa30.map 1 1 8 13 0 11 18 0 18 12
        ;;
    *)
        echo "Argumento no válido: $1. Por favor, usa un número del 1 al 6."
        exit 2
        ;;
esac
