#ifndef HASHING_CERRADO_H
#define HASHING_CERRADO_H

#include <string>
#include <vector>

// Mezcla de implementación vista en capsula de Laboratorio 9 y el documento ht_cerrado.cpp de ese mismo lab.
// Con modificaciones de comportamiento, ahora es un contador de frecuencia. 

// Estados de los nodos
enum State
{
    EMPTY,
    OCCUPIED,
    DELETED
};

// Estructura para cada nodo de la tabla.
struct HashNode
{
    std::string key;
    int value;
    State state;

    HashNode() : key(""), value(0), state(EMPTY) {}
};

// Constante global externa para el método h2
extern const float A;

// Funciones hash base
int h1(const std::string &k, int n);
int h2(const std::string &k, int n);

// Métodos de probing
int linear_probing(const std::string &k, int n, int i);
int quadratic_probing(const std::string &k, int n, int i);
int double_hashing(const std::string &k, int n, int i);

// Definición de la clase HashTable
class HashTable
{
private:
    int size;
    std::vector<HashNode> table;

    int (*hashing_method)(const std::string&, int, int);

public:
    // Constructor
    HashTable(int size, int (*hashing_method)(const std::string&, int, int));

    // soporta user_id y user_screen_name
    void insert(const std::string &key);
    int get(const std::string &key);
    void remove(const std::string &key);
};

#endif // HASHING_CERRADO_H