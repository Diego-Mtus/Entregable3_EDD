
# 2. Definir los arreglos de configuración
# Estructuras de datos a evaluar
estructuras=("abierto" "lineal" "cuadratico" "doble")

# Datasets: 1 para user_ids, 0 para user_screen_names
datasets=(1 0)

echo "numero_experimento;dataset;estructura_de_datos;cantidad_consultas;tiempo_ejecucion"

# 3. Bucle principal de tamaños (de 10,000 a 180,000 en pasos de 10,000)
for cantidad in {10000..180000..10000}
do
    for est in "${estructuras[@]}"
    do
        for ds in "${datasets[@]}"
        do
            # Repetir cada combinación exactamente 20 veces
            for repeticion in {1..20}
            do
                # Ejecutar el programa pasándole los 4 argumentos:
                # <cantidad> <estructura> <1 o 0> <num_experimento>
                ./experimento $cantidad "$est" $ds $repeticion
                
            done
        done
    done
done