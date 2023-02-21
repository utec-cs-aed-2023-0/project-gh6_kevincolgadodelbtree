#include <iostream>
#include <vector>

using namespace std;

class BPlusNode 
{
private:
    vector<int> keys;  //Las claves en el nodo
    bool is_leaf;      //Retorna true si el nodo es una hoja, false si es un nodo interno
    BPlusNode* parent; //Puntero que va al nodo padre, no hay misterio
    vector<BPlusNode*> children; //Punteros que van a los hijos del nodo

public:
bool search(BPlusNode* root, int value) {
    //Si el nodo es una hoja, buscar el valor en sus claves
    if (root->is_leaf) {
        for (int key : root->keys) {
            if (key == value) {
                return true;
            }
        }
        return false;
    }
    //Si el nodo es un nodo interno, 
    //busca el hijo correspondiente para continuar la búsqueda
    else {
        int i = 0;
        while (i < root->keys.size() && root->keys[i] < value) {
            i++;
        }
        return search(root->children[i], value);
    }
}

void insert(BPlusNode* root, int value) {
    //Si el nodo es una hoja, insertar el valor en orden
    if (root->is_leaf) {
        int i = 0;
        while (i < root->keys.size() && root->keys[i] < value) {
            i++;
        }
        root->keys.insert(root->keys.begin() + i, value);
    }
    // Si el nodo es un nodo interno, buscar el hijo correspondiente para continuar la inserción
    else {
        int i = 0;
        while (i < root->keys.size() && root->keys[i] < value) {
            i++;
        }
        insert(root->children[i], value);
    }
}

};

