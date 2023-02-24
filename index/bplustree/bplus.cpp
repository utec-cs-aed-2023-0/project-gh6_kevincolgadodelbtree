
#include "bplus.h"
using namespace std;

//nota al consumidor (osea jdp):
//asume que los nodos del árbol B+ tienen una capacidad 
//máxima fija y que se utilizan punteros para almacenar referencias 
//a los nodos y sus claves.

int main() {
    // Crear un árbol B+ vacío
    BPlusTree btree(5);
    
    // Insertar algunos valores en el árbol B+
    btree.insert(10);
    btree.insert(5);
    btree.insert(20);
    btree.insert(15);
    
    // Buscar algunos valores en el árbol B+
    cout << btree.search(10) << endl; // Debe imprimir 1
    cout << btree.search(7) << endl;  // Debe imprimir 0
    cout << btree.search(15) << endl; // Debe imprimir 1
    
    return 0;
}