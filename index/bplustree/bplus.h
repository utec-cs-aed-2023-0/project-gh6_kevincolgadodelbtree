#include <iostream>
#include <vector>

using namespace std;

// Nodo interno del árbol B+
class BPlusNode {
public:
    vector<int> keys;  //Las claves en el nodo
    bool is_leaf;      //Retorna true si el nodo es una hoja, false si es un nodo interno
    BPlusNode* parent; //Puntero que va al nodo padre, no hay misterio
    vector<BPlusNode*> children; //Punteros que van a los hijos del nodo

    //Constructor
    BPlusNode(bool is_leaf) {
        this->is_leaf = is_leaf;
    }

    BPlusNode() = default;

};

class BPlusTree {
private:
    BPlusNode* root;
    int degree; //los grados vaya q sorpresa

public:
    //Constructor
    BPlusTree(int degree) {
        this->root = NULL;
        this->degree = degree;
    }

    //Destructor
    ~BPlusTree() {
        ultrasoloremix(root);
    }

    //Elimina todo el árbol recursivamente
    //nombre provisional, es el destroyer del arbol xd
    void ultrasoloremix(BPlusNode* node) {
        if (node == NULL) return;
        if (node->is_leaf) {
            delete node;
            return;
        }
        for (int i = 0; i < node->children.size(); i++) {
            ultrasoloremix(node->children[i]);
            //gracias jd por la idea uwu
        }
        delete node;
    }

    //Búsqueda de una llave en el árbol
    BPlusNode* search(int key) {
        return search(root, key);
    }

    //Esto deberia ir en el private pero ya q xu
    BPlusNode* search(BPlusNode* node, int key) {
        if (node == NULL) return NULL;
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        if (node->is_leaf) {
            if (i < node->keys.size() && node->keys[i] == key) {
                return node;
            }
            return NULL;
        } else {
            return search(node->children[i], key);
        }
    }

    //Insert de una llave en el árbol
    void insert(int key) {
        if (root == NULL) {
            root = new BPlusNode(true);
            root->keys.push_back(key);
        } else {
            if (root->keys.size() == degree * 2 - 1) {
                BPlusNode* new_root = new BPlusNode(false);
                new_root->children.push_back(root);
                //OJO ACA VA:
                //split_child(new_root, 0, root);
                //Pero está comentado porq no tengo aun el split
                root = new_root;
            }
            //Este insert se explica abajo
            insert_no_full(root, key);
        }
    }


    void insert_no_full(BPlusNode* node, int key) {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        if (node->is_leaf) {
            node->keys.insert(node->keys.begin() + i, key);
        } else {
            if (node->children[i]->keys.size() == degree * 2 - 1) {
                //OJO ACA VA:
                //split_child(node, i, node->children[i]);
                //Pero está comentado porq no tengo aun el split
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insert_no_full(node->children[i], key);
        }
    }

    /*
    //División de un nodo hijo de forma recursiva? Aun no se xd
    void split_child(BPlusNode* parent, int i, BPlusNode* child) {
        BPlusNode* new_child = new BPlusNode

    //Ayuden hijos de la maracaaaaaaa no tengo ideaaaaaaaaaa
    */
};

