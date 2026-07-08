#include "hashing_cerrado.h"
#include <cmath>
#include <numeric>
#include <cctype>

/*

Las funciones hash implementadas son las mismas que se vieron en la ayudantía del lab 9, con la adición del caché para no recalcular
tantas veces, además que se usa std::accumulate(k.begin(), k.end(), 0); al calcular el hash de un string.

Todas las veces que se utiliza esa función es para transformar user_screen_name a números, mediante 
la suma de los valores ASCII de sus caracteres, y para user_id se transforma a número directamente.

*/

// Definición de la constante A para h2
const float A = (std::sqrt(5) - 1) / 2;

// Auxiliar para determinar si es número o string
bool es_numero(const std::string &s)
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

// funciones de hashing base
int h1(const std::string &k, int n)
{
 
    // puse que se guardara en cache el último resultado para que no tenga que recalcularlo todo el tiempo con la misma entrada
    static std::string ultimo_k = "";
    static int ultimo_n = -1;
    static int ultimo_resultado = -1;

    if (k == ultimo_k && n == ultimo_n) {
        return ultimo_resultado;
    }

    int resultado = 0;
    if (es_numero(k))
    {
        long long numero = 0;
        for (char c : k)
        {
            numero = numero * 10 + (c - '0');
        }
        resultado = std::abs(numero) % n;
    }
    else
    {
        int suma_ascii = std::accumulate(k.begin(), k.end(), 0);
        resultado = suma_ascii % n;
    }

    // Actualizar caché
    ultimo_k = k;
    ultimo_n = n;
    ultimo_resultado = resultado;
    return resultado;
}

int h2(const std::string &k, int n)
{
    static std::string ultimo_k = "";
    static int ultimo_n = -1;
    static int ultimo_resultado = -1;

    if (k == ultimo_k && n == ultimo_n) {
        return ultimo_resultado;
    }

    long long valor_numerico = 0;
    if (es_numero(k))
    {
        for (char c : k)
        {
            valor_numerico = valor_numerico * 10 + (c - '0');
        }
        valor_numerico = std::abs(valor_numerico);
    }
    else
    {
        valor_numerico = std::accumulate(k.begin(), k.end(), 0);
    }

    float a = (float)valor_numerico * A;
    a -= (int)a;
    
    int resultado = n * a;


    ultimo_k = k;
    ultimo_n = n;
    ultimo_resultado = resultado;
    return resultado;
}

// Métodos de direccionamiento abierto
int linear_probing(const std::string &k, int n, int i)
{
    long long posicion = static_cast<long long>(h2(k, n)) + i;
    return std::abs(posicion) % n;
}

int quadratic_probing(const std::string &k, int n, int i)
{
    // 2LL para evitar overflow
    long long posicion = static_cast<long long>(h2(k, n)) + i + 2LL * i * i;
    return std::abs(posicion) % n;
}

int double_hashing(const std::string &k, int n, int i)
{
    long long posicion = static_cast<long long>(h2(k, n)) + static_cast<long long>(i) * (h1(k, n) + 1);
    return std::abs(posicion) % n;
}

// Constructor de HashTable
HashTable::HashTable(int size, int (*hashing_method)(const std::string &, int, int))
    : size(size), hashing_method(hashing_method)
{
    table.resize(size);
}

// Insertar maneja user_id y user_screen_name
void HashTable::insert(const std::string &key)
{
    int aux_index = -1;

    for (int i = 0; i < size; i++)
    {
        int index = hashing_method(key, size, i);

        // Si encontramos la clave, incrementamos su valor y retornamos
        if (table[index].state == OCCUPIED && table[index].key == key)
        {
            table[index].value++;
            return;
        }

        // Si encontramos un índice vacío o eliminado, lo guardamos como posible lugar para insertar
        if (table[index].state == EMPTY || table[index].state == DELETED)
        {
            if (aux_index == -1)
            {
                aux_index = index;
            }
        }

        // Si llegamos a un índice vacío, significa que la clave no está en la tabla y podemos insertar en aux_index
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

// Obtener maneja user_id y user_screen_name
int HashTable::get(const std::string &key)
{
    for (int i = 0; i < size; i++)
    {
        int index = hashing_method(key, size, i);

        // Si encontramos un índice vacío, significa que la clave no está en la tabla
        if (table[index].state == EMPTY)
        {
            break;
        }

        // Si encontramos la clave, retornamos su valor
        if (table[index].state == OCCUPIED && table[index].key == key)
        {
            return table[index].value;
        }

        // Si se encuentra un índice eliminado u ocupado con otra clave, continuamos buscando
    }
    return -1;
}

// Eliminar maneja user_id y user_screen_name
void HashTable::remove(const std::string &key)
{
    for (int i = 0; i < size; i++)
    {
        int index = hashing_method(key, size, i);

        // Si encontramos un índice vacío, significa que la clave no está en la tabla
        if (table[index].state == EMPTY)
        {
            break;
        }

        // Si encontramos la clave, la marcamos como eliminada
        // Cabe destacar que no se borra físicamente el nodo/su información, solo se marca como DELETED.
        // Esto hará que se sobreescriba si se inserta una nueva clave ahí.
        if (table[index].state == OCCUPIED && table[index].key == key)
        {
            table[index].state = DELETED;
            return;
        }
    }
}