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
    long long key;
    int value;
    State state;

    HashNode() : key(0), value(0), state(EMPTY) {}
};

// Constante global externa para el método h2
extern const float A;

// Funciones hash
int h1(int k, int n);
int h2(int k, int n);

// Métodos de probing
int linear_probing(int k, int n, int i);
int quadratic_probing(int k, int n, int i);
int double_hashing(int k, int n, int i);

// Definición de la clase HashTable
class HashTable
{
private:
    int size;
    std::vector<HashNode> table;
    int (*hashing_method)(int, int, int);

public:
    // Constructor
    HashTable(int size, int (*hashing_method)(int, int, int));

    // Inserción
    void insert(long long key);
    void insert(std::string key);

    // Búsqueda
    int get(long long key);
    int get(std::string key);

    // Eliminación
    void remove(long long key);
    void remove(std::string key);
};

#endif // HASHING_CERRADO_H