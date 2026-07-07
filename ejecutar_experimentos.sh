#!/bin/bash

# Inicializamos
estructuras=("abierto" "lineal" "cuadratico" "doble" "unordered_map")
datasets=(1 0)

echo "numero_experimento;dataset;estructura_de_datos;cantidad_consultas;tiempo_ejecucion"

# De 10000 en 10000
for cantidad in {10000..180000..10000}
do
    for est in "${estructuras[@]}"
    do
        for ds in "${datasets[@]}"
        do
            # Se repite 20 veces
            for repeticion in {1..20}
            do
                ./experimento $cantidad "$est" $ds $repeticion
            done
        done
    done
done