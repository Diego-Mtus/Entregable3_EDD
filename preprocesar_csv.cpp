#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// created_at,id,full_text,retweet_count,favorite_count,user_id,user_name,user_screen_name,user_description,user_location,user_created_at

// Para parsear cada linea del csv, respetando las comillas dobles, considerando que a veces
// el csv puede tener comas dentro de las comillas dobles.
std::vector<std::string> parsear_linea(const std::string &linea)
{
    std::vector<std::string> resultado;
    std::string campo_actual = "";
    bool in_quotes = false;

    for (char c : linea)
    {
        if (c == '"')
        {
            in_quotes = !in_quotes; // Si hay una comilla doble, se alterna el estado de comilla
        }
        else if (c == ',' && !in_quotes)
        {
            resultado.push_back(campo_actual);
            campo_actual = "";
        }
        else
        {
            campo_actual += c;
        }
    }
    resultado.push_back(campo_actual); // Agregar el último campo
    return resultado;
}

int main()
{
    std::ifstream entrada("auspol2019.csv");
    std::ofstream salida_id("user_ids.txt");
    std::ofstream salida_name("user_screen_names.txt");

    if (!entrada.is_open() || !salida_id.is_open() || !salida_name.is_open())
    {
        std::cerr << "Error al abrir los archivos de entrada o salida." << std::endl;
        return 1;
    }

    std::string linea_fisica;
    std::string registro_completo = "";
    bool comillas_abiertas = false;

    std::getline(entrada, linea_fisica); // Descartar cabecera del csv

    std::cout << "Procesando CSV..." << std::endl;

    long long count = 0;

    // Hay que considerar que un tweet puede estar en varias lineas, por lo que se debe ir concatenando hasta que se cierren las comillas dobles.
    while (std::getline(entrada, linea_fisica))
    {
        // Si el registro estaba vacío y no estamos en medio de un tweet entrecomillado, se salta
        if (linea_fisica.empty() && !comillas_abiertas)
            continue;

        // Si es la continuación de una línea anterior, añadimos un salto de línea real
        if (!registro_completo.empty()) {
            registro_completo += "\n";
        }
        registro_completo += linea_fisica;

        // Contamos cuántas comillas hay en esta línea para saber si se cerraron o siguen abiertas
        for (char c : linea_fisica) {
            if (c == '"') {
                comillas_abiertas = !comillas_abiertas;
            }
        }

        // Si las comillas están cerradas (false), significa que se tiene un registro completo para procesar.
        if (!comillas_abiertas) {
            std::vector<std::string> campos = parsear_linea(registro_completo);

            if (campos.size() >= 8)
            {
                salida_id << campos[5] << "\n";   // user_id
                salida_name << campos[7] << "\n"; // user_screen_name
                count++;
            }

            // Limpiar buffer
            registro_completo = "";
        }
    }

    entrada.close();
    salida_id.close();
    salida_name.close();

    std::cout << "Se procesaron de forma limpia: " << count << " líneas." << std::endl;

    return 0;
}