#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sys/resource.h>

#include "hashing_cerrado.h"
#include "hashing_abierto.h"

// Función auxiliar para obtener los KB exactos de RAM usados
long get_memoria_kb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // En Linux retorna KB directos
}


int main(int argc, char* argv[]) {
    // Formato es <ejecutable> <cantidad> <estructura_de_datos> <1: id / 0: screen_name>
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " <cantidad> <estructura_de_datos> <1: user_id / 0: screen_name>\n";
        std::cerr << "Estructuras: abierto, lineal, cuadratico, doble, unordered_map\n";
        return 1;
    }

    // Parsear los argumentos de la línea de comandos
    long long cantidad_lineas = std::stoll(argv[1]);
    std::string estructura = argv[2];
    bool usar_id = (std::stoi(argv[3]) == 1);

    // Seleccionar el archivo correspondiente
    std::string nombre_archivo = usar_id ? "user_ids.txt" : "user_screen_names.txt";
    std::ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombre_archivo << "\n";
        return 1;
    }

    // Se cargan solo las líneas solicitadas para aislar la memoria de la tabla
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

    // factor de carga fija
    int tamano_tabla = 65521;

    // Medimos la memoria base justo antes de crear la estructura
    long memoria_base = get_memoria_kb();
    long memoria_final = 0;

    // Inicializar la estructura elegida y ejecutar el experimento
    if (estructura == "abierto") {
        HashTableAbierto tabla(tamano_tabla);
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        memoria_final = get_memoria_kb();

    } else if (estructura == "lineal") {
        HashTable tabla(tamano_tabla, linear_probing);
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        memoria_final = get_memoria_kb();

    } else if (estructura == "cuadratico") {
        HashTable tabla(tamano_tabla, quadratic_probing);
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        memoria_final = get_memoria_kb();

    } else if (estructura == "doble") {
        HashTable tabla(tamano_tabla, double_hashing);
        for (const auto& dato : datos) {
            tabla.insert(dato);
        }
        memoria_final = get_memoria_kb();

    } else if (estructura == "unordered_map") {
        std::unordered_map<std::string, int> tabla;
        tabla.reserve(tamano_tabla);
        for (const auto& dato : datos) {
            tabla[dato]++;
        }
        memoria_final = get_memoria_kb();
    } else {
        std::cerr << "Error: Estructura de datos no válida ('" << estructura << "').\n";
        return 1;
    }

    // Calcular el consumo de memoria neta
    long memoria_consumida = memoria_final - memoria_base;

    
    std::string dataset = usar_id ? "user_ids" : "user_screen_names";

    std::cout << dataset << ";"
              << estructura << ";"
              << cantidad_lineas << ";"
              << memoria_consumida << std::endl; // Salida limpia en KB para tu script de Python

    return 0;
}