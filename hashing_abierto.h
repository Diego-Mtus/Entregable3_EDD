#ifndef HASHING_ABIERTO_H
#define HASHING_ABIERTO_H

#include <string>
#include <vector>
#include <list>

// Estructura para cada nodo de la tabla.
struct HashNodeAbierto
{

    // Se decidió usar un string como clave para contemplar user_screen_name y user_id.
    std::string key;
    int value;

    HashNodeAbierto() : key(""), value(0) {}
};

class HashTableAbierto
{
private:
    int size;
    std::vector<std::list<HashNodeAbierto>> table;

    // Métodos privados de hashing internos
    bool es_numero(const std::string &s);
    int hashing_method(const std::string &k);

public:
    // Constructor
    HashTableAbierto(int size);

    // Interfaz pública
    void insert(const std::string &key);
    int get(const std::string &key);
    void remove(const std::string &key);
};

#endif // HASHING_ABIERTO_H