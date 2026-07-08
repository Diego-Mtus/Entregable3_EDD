#include "hashing_abierto.h"
#include <numeric>
#include <cmath>
#include <cctype>

// Métodos privados de hashing internos

// Retorna true si la cadena es un número, false en caso contrario.
bool HashTableAbierto::es_numero(const std::string &s)
{
    if (s.empty())
        return false;
    for (char c : s)
    {
        if (!std::isdigit(c))
            return false;
    }
    return true;
}

// El hashing method toma la key y devuelve un índice en la tabla de hash. 
// Si la key es un número, se convierte a entero y se aplica el módulo con el tamaño de la tabla. 
// Si no es un número, se calcula la suma de los valores ASCII de los caracteres de la key y se aplica el módulo con el tamaño de la tabla.
int HashTableAbierto::hashing_method(const std::string &k)
{
    if (es_numero(k))
    {
        long long numero = std::stoll(k);
        return std::abs(numero) % size;
    }

    int suma_ascii = std::accumulate(k.begin(), k.end(), 0);
    return suma_ascii % size;
}

HashTableAbierto::HashTableAbierto(int size) : size(size)
{
    table.resize(size);
}

void HashTableAbierto::insert(const std::string &key)
{
    int index = hashing_method(key);

    for (auto &node : table[index])
    {

        // El valor se incrementa si ya existe en la tabla
        if (node.key == key)
        {
            node.value++;
            return;
        }
    }


    // EN otro caso, se crea el nodo con valor 1
    HashNodeAbierto newNode;
    newNode.key = key;
    newNode.value = 1;
    table[index].push_back(newNode);
}

int HashTableAbierto::get(const std::string &key)
{
    int index = hashing_method(key);

    for (const auto &node : table[index])
    {

        // Retorna el valor si la clave existe en la tabla
        if (node.key == key)
        {
            return node.value;
        }
    }

    return -1;
}

void HashTableAbierto::remove(const std::string &key)
{
    int index = hashing_method(key);

    // Se obtiene la lista de nodos en ese índice para poder borrarlo si se encuentra la clave
    auto &lista_de_index = table[index];
    for (auto it = lista_de_index.begin(); it != lista_de_index.end(); ++it)
    {
        if (it->key == key)
        {
            lista_de_index.erase(it);
            return;
        }
    }
}