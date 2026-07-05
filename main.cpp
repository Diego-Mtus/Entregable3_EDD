#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <unordered_map>
#include "hashing_abierto.h"
#include "hashing_cerrado.h"


// Se usó de referencia la clase online "Experimentación-20240517.mp4"
// https://drive.google.com/file/d/1hxDNnpJrgYwXbO15Bllzr84282414nFb/view
int main(int argc, char* argv[])
{
    // Formato es <ejecutable> <cantidad> <estructura_de_datos> <1: id / 0: screen_name> <numero_experimento>
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " <cantidad> <estructura_de_datos> <1: user_id / 0: screen_name> <numero_experimento>\n";
        std::cerr << "Estructuras: abierto, lineal, cuadratico, doble, unordered_map\n";
        return 1;
    }

    // 1. Parsear los argumentos de la línea de comandos
    long long cantidad_lineas = std::stoll(argv[1]);
    std::string estructura = argv[2];
    bool usar_id = (std::stoi(argv[3]) == 1);
    int numero_experimento = std::stoi(argv[4]);

    // 2. Seleccionar el archivo correspondiente
    std::string nombre_archivo = usar_id ? "user_ids.txt" : "user_screen_names.txt";
    std::ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombre_archivo << "\n";
        return 1;
    }

    // Se cargan solo las lineas solicitadas
    std::vector<std::string> datos;
    datos.reserve(cantidad_lineas);
    std::string linea;
    long long leidas = 0;

    while (leidas < cantidad_lineas && std::getline(archivo, linea)) {
        if (!linea.empty()) {
            datos.push_back(linea);
            leidas++;
        }
    }
    archivo.close();

    // factor de carga 0.7 respecto a los valores únicos (justificacion en readme)
    int tamano_tabla = 65521;

    
    // Variables para la medición de tiempo
    std::chrono::high_resolution_clock::time_point inicio;
    std::chrono::high_resolution_clock::time_point fin;

    // 3. Inicializar la estructura elegida y ejecutar el experimento
    if (estructura == "abierto") {
        HashTableAbierto tabla(tamano_tabla);

        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        fin = std::chrono::high_resolution_clock::now();

    } else if (estructura == "lineal") {
        HashTable tabla(tamano_tabla, linear_probing);

        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        fin = std::chrono::high_resolution_clock::now();

    } else if (estructura == "cuadratico") {
        HashTable tabla(tamano_tabla, quadratic_probing);

        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        fin = std::chrono::high_resolution_clock::now();

    } else if (estructura == "doble") {
        HashTable tabla(tamano_tabla, double_hashing);

        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        fin = std::chrono::high_resolution_clock::now();

    } else if (estructura == "unordered_map") {
        std::unordered_map<std::string, int> tabla;
        tabla.reserve(tamano_tabla); // Reservamos espacio para la comparación más justa.

        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& dato : datos) {
            tabla[dato]++; 
        }
        fin = std::chrono::high_resolution_clock::now();
    }
    else {
        std::cerr << "Error: Estructura de datos no válida ('" << estructura << "').\n";
        return 1;
    }

    // 4. Calcular el tiempo en segundos
    double tiempo_ejecucion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio).count();
    tiempo_ejecucion *= 1e-9; // Convertir a segundos


    // 5. Imprimir la salida con el formato solicitado
    std::string dataset = usar_id ? "user_ids" : "user_screen_names";

    std::cout << numero_experimento << ";"
              << dataset << ";"
              << estructura << ";"
              << cantidad_lineas << ";"
              << tiempo_ejecucion << std::endl;

    return 0;
}