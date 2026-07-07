#!/bin/bash
echo "estructura;tipo_llave;cantidad_datos;memoria_usada" > reporte_memoria.csv

for est in "lineal" "cuadratico" "doble" "abierto" "unordered_map"
do
    ./medir_memoria 180000 "$est" 1 >> reporte_memoria.csv
    ./medir_memoria 180000 "$est" 0 >> reporte_memoria.csv
done