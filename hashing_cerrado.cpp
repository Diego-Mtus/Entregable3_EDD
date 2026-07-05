#include "hashing_cerrado.h"

#include <cmath>
#include <numeric>

// Definición de la constante A para h2
const float A = (std::sqrt(5) - 1) / 2;

// Implementación de funciones de hashing
int h1(int k, int n)
{
    return k % n;
}

int h2(int k, int n)
{
    float a = (float)k * A;
    a -= (int)a;
    return n * a;
}

int linear_probing(int k, int n, int i)
{
    return (h2(k, n) + i) % n;
}

int quadratic_probing(int k, int n, int i)
{
    return (h2(k, n) + i + 2 * i * i) % n;
}

int double_hashing(int k, int n, int i)
{
    return (h2(k, n) + i * (h1(k, n) + 1)) % n;
}

// Constructor de HashTable
HashTable::HashTable(int size, int (*hashing_method)(int, int, int))
    : size(size), hashing_method(hashing_method)
{
    table.resize(size);
}

// Insertar entero
void HashTable::insert(long long key)
{
    int aux_index = -1;

    for (int i = 0; i < size; i++)
    {
        int index = hashing_method(key, size, i);

        if (table[index].state == OCCUPIED && table[index].key == key)
        {
            table[index].value++;
            return;
        }

        if (table[index].state == EMPTY || table[index].state == DELETED)
        {
            if (aux_index == -1)
            {
                aux_index = index;
            }
        }

        if (table[index].state == EMPTY)
        {
            break;
        }
    }

    if (aux_index != -1)
    {
        table[aux_index].key = key;
        table[aux_index].value = 1;
        table[aux_index].state = OCCUPIED;
        return;
    }
}

// Insertar string (con overloading)
void HashTable::insert(std::string key)
{
    int suma_ascii = std::accumulate(key.begin(), key.end(), 0);
    insert(suma_ascii);
}

// Obtener por entero
int HashTable::get(long long key)
{
    for (int i = 0; i < size; i++)
    {
        int index = hashing_method(key, size, i);
        if (table[index].state == EMPTY)
        {
            break;
        }

        if (table[index].state == OCCUPIED && table[index].key == key)
        {
            return table[index].value;
        }
    }
    return -1;
}

// Obtener por string (con overloading)
int HashTable::get(std::string key)
{
    int suma_ascii = std::accumulate(key.begin(), key.end(), 0);
    return get(suma_ascii);
}

// Eliminar por entero
void HashTable::remove(long long key)
{
    for (int i = 0; i < size; i++)
    {
        int index = hashing_method(key, size, i);
        if (table[index].state == EMPTY)
        {
            break;
        }

        if (table[index].state == OCCUPIED && table[index].key == key)
        {
            table[index].state = DELETED;
            return;
        }
    }
}

// Eliminar por string (con overloading)
void HashTable::remove(std::string key)
{
    int suma_ascii = std::accumulate(key.begin(), key.end(), 0);
    remove(suma_ascii);
}