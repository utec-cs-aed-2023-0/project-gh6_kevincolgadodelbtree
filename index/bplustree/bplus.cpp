
#include "bplus.h"
using namespace std;

//nota al consumidor (osea jdp):
//asume que los nodos del árbol B+ tienen una capacidad 
//máxima fija y que se utilizan punteros para almacenar referencias 
//a los nodos y sus claves.

int main() {
    // Crear un árbol B+ vacío
    BPlusNode* root = new BPlusNode();
    root->is_leaf = true;
    root->parent = nullptr;
    
    // Insertar algunos valores en el árbol B+
    insert(root, 10);
    insert(root, 5);
    insert(root, 20);
    insert(root, 15);
    
    // Buscar algunos valores en el árbol B+
    cout << search(root, 10) << endl; // Debe imprimir 1
    cout << search(root, 7) << endl;  // Debe imprimir 0
    cout << search(root, 15) << endl; // Debe imprimir 1
    
    return 0;
}