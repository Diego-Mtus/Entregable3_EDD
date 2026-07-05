# Entregable 3: Tablas Hash 

## Consideraciones

A partir del análisis de la página del dataset, se determinó que existen exactamente **45,863 valores únicos** para la variable `user_screen_name` (y, por consiguiente, para `user_id`). Con el objetivo de evaluar y comparar el rendimiento de las distintas implementaciones de tablas hash bajo condiciones equivalentes, se define un tamaño estático para su creación basado en los siguientes criterios:


1) **Factor de carga objetivo:** Se busca un factor de carga máximo de aproximadamente un 70% ($\alpha = 0.7$) al insertar la última llave.

$$N_{ideal} = \frac{45863}{0.7} \approx 65518.57$$

2) Para garantizar que el double hashing pueda explorar sin caer en ciclos infinitos, el tamaño **N** debe ser obligatoriamente un número primo.

Por lo tanto, el tamaño escogido para todas las pruebas es:

$$ N = 65521$$

### Nota sobre Hashing Abierto
Esta consideración se mantiene para la evaluación de **hashing abierto**, para hacer una comparativa "justa" en terminos de memoria asignada inicialmente. Esto causaría que el método de chaining sea muy rápido, pues, la longitud promedio de las listas enlazadas en cada posición será muy baja (cercana a 1), minimizando las búsquedas lineales consecutivas.

