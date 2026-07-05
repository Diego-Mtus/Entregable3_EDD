#include "hashing_abierto.h"
#include <numeric>
#include <cmath>
#include <cctype>

bool HashTableAbierto::es_numero(const std::string &s)
{
    if (s.empty()) return false;
    for (char c : s)
    {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

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
        if (node.key == key)
        {
            node.value++;
            return;
        }
    }

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