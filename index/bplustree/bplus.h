#include <iostream>
#include <vector>

using namespace std;

const int MAX_KEYS = 3;
const int MAX_CHILDREN = MAX_KEYS + 1;

class BPlusTree {
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
            //gracias jd por la idea
        }
        delete node;
    }

    void insert(int key);
    void remove(int key);
    void search(int key);
    void print();

private:
    struct BPlusNode {
        int num_keys;
        bool is_leaf;   //Retorna true si el nodo es una hoja, false si es un nodo interno
        vector<int> keys;   //Las claves en el nodo
        BPlusNode* parent; //Puntero que va al nodo padre, no hay misterio
        vector<BPlusNode*> children; //Punteros que van a los hijos del nodo

        BPlusNode(bool _is_leaf) {
            num_keys = 0;
            is_leaf = _is_leaf;
            keys.resize(MAX_KEYS);
            children.resize(MAX_CHILDREN);
        }
    };

    BPlusNode* root;
    void split_child(BPlusNode* parent, int child_idx);
    void insert_non_full(BPlusNode* node, int key);
    void remove_non_leaf(BPlusNode* node, int key);
    void remove_from_leaf(BPlusNode* node, int key);
    void borrow_from_sibling(BPlusNode* node, int idx);
    void merge_with_sibling(BPlusNode* node, int idx);
    BPlusNode* search_node(int key);
    void print_recursive(BPlusNode* node, int level);
};

BPlusTree::BPlusTree() {
    root = new BPlusNode(true);
}

void BPlusTree::insert(int key) {
    if (root->num_keys == MAX_KEYS) {
        BPlusNode* new_root = new BPlusNode(false);
        new_root->children[0] = root;
        root = new_root;
        split_child(root, 0);
    }
    insert_non_full(root, key);
}

void BPlusTree::insert_non_full(BPlusNode* node, int key, int value) {
    int i = node->num_keys - 1;
    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->num_keys++;
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == max_keys_per_node) {
            split_child(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key, value);
    }
}
/*
Esta función se llama cuando se desea insertar una clave en un nodo que no está lleno. 
Si el nodo es una hoja, simplemente se inserta la clave y el valor en el lugar apropiado del nodo. 
Si el nodo no es una hoja, se encuentra el hijo apropiado del nodo para insertar la clave, 
y si el hijo está lleno, se realiza una división del hijo antes de continuar la inserción. 
Continúa recursivamente hasta que se inserta la clave en un nodo hoja.
*/

void BPlusTree::split_child(BPlusNode* parent, int child_idx) {
    BPlusNode* child = parent->children[child_idx];
    BPlusNode* new_child = new BPlusNode(child->is_leaf);
    new_child->num_keys = MAX_KEYS / 2;
    for (int i = 0; i < new_child->num_keys; i++) {
        new_child->keys[i] = child->keys[i + MAX_KEYS / 2];
    }
    if (!child->is_leaf) {
        for (int i = 0; i <= new_child->num_keys; i++) {
            new_child->children[i] = child->children[i + MAX_KEYS / 2];
        }
    }
    child->num_keys = MAX_KEYS / 2;
    for (int i = parent->num_keys; i > child_idx; i--) {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }
    parent->children[child_idx + 1] = new_child;
    parent->keys[child_idx] = child->keys[MAX_KEYS / 2 - 1];
    parent->num_keys++;
}

void BPlusTree::remove(BPlusNode* node, int key) {
    int idx = find_index(node, key);
    if (idx < node->num_keys && node->keys[idx] == key) {
        if (node->is_leaf) {
            remove_from_leaf(node, idx);
        } else {
            remove_from_non_leaf(node, idx);
        }
    } else {
        if (node->is_leaf) {
            std::cout << "La clave " << key << " no existe en el árbol." << std::endl;
        } else {
            bool flag = (idx==node->num_keys);
            if (node->children[idx]->num_keys < min_keys) {
                fill(idx);
            }
            if (flag && idx > node->num_keys) {
                remove(node->children[idx-1], key);
            } else {
                remove(node->children[idx], key);
            }
        }
    }
}
/*
Esta función se utiliza para eliminar una clave del árbol. La función toma como argumentos el nodo 
y la clave que se van a eliminar. Primero, la función busca el índice donde debería estar la clave. 
Si la clave se encuentra en el nodo actual, la función determina si el nodo es una hoja o no. 
Si el nodo es una hoja, llama a remove_from_leaf para eliminar la clave del nodo. 
Si el nodo no es una hoja, llama a remove_from_non_leaf para eliminar la clave del nodo. 
Si la clave no se encuentra en el nodo actual, la función busca el hijo donde la clave podría estar 
y llama a sí misma recursivamente en ese hijo. Si el número de claves en el hijo se reduce por 
debajo del mínimo permitido, la función llama a fill para corregir el subárbol.
*/

void BPlusTree::remove_non_leaf(BPlusNode* node, int key) {
    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (key < node->keys[i]) {
            break;
        }
    }
    BPlusNode* child = node->children[i];
    if (child->num_keys > MAX_KEYS / 2) {
        remove(child, key);
    } else {
        if (i > 0 && node->children[i - 1]->num_keys > MAX_KEYS / 2) {
            borrow_from_sibling(child, i - 1);
        } else if (i < node->num_keys && node->children[i + 1]->num_keys > MAX_KEYS / 2) {
            borrow_from_sibling(child, i + 1);
        } else {
            if (i > 0) {
                merge_with_sibling(node, i - 1);
                child = node->children[i - 1];
            } else {
                merge_with_sibling(node, i);
            }
            remove(child, key);
        }
    }
}
/*
Esta función busca la posición de la clave a eliminar en el nodo y luego verifica si el hijo 
correspondiente tiene suficientes claves para poder realizar la eliminación en él. 
Si el hijo no tiene suficientes claves, se intenta tomar prestado una clave de un hermano que 
tenga más claves. Si no se puede tomar prestado ninguna clave, se realiza una fusión con un hermano 
que tenga menos claves y se elimina la clave del hijo fusionado.
*/

void BPlusTree::remove_from_leaf(BPlusNode* node, int key) {
    int i = 0;
    while (i < node->num_keys && node->keys[i] != key) {
        i++;
    }
    if (i == node->num_keys) {
        return;
    }
    for (int j = i; j < node->num_keys - 1; j++) {
        node->keys[j] = node->keys[j + 1];
        node->values[j] = node->values[j + 1];
    }
    node->num_keys--;
    if (node == root) {
        if (node->num_keys == 0) {
            if (!node->is_leaf) {
                root = node->children[0];
            } else {
                root = nullptr;
            }
            delete node;
        }
        return;
    }
    if (node->num_keys >= MIN_KEYS / 2) {
        return;
    }
    BPlusNode* parent = node->parent;
    int pos = 0;
    while (pos <= parent->num_keys && parent->children[pos] != node) {
        pos++;
    }
    if (pos == parent->num_keys + 1) {
        pos--;
    }
    if (pos > 0 && parent->children[pos - 1]->num_keys > MIN_KEYS / 2) {
        borrow_from_sibling(node, pos - 1);
    } else if (pos < parent->num_keys && parent->children[pos + 1]->num_keys > MIN_KEYS / 2) {
        borrow_from_sibling(node, pos + 1);
    } else {
        if (pos > 0) {
            merge_with_sibling(node, pos - 1);
            node = parent->children[pos - 1];
        } else {
            merge_with_sibling(node, pos);
        }
        remove_from_leaf(node, key);
    }
}
/*
La función esta busca la posición de la clave a eliminar en el nodo hoja y luego la elimina del 
arreglo de claves y valores del nodo. Si el nodo hoja tiene menos claves que el mínimo permitido, 
se intenta tomar prestado una clave de un hermano que tenga más claves con la func borrow de abajo. 
Si no se puede tomar prestado ninguna clave, se realiza una fusión con un hermano que tenga menos 
claves y se vuelve a intentar eliminar la clave en el nodo fusionado. Si el nodo hoja es la raíz y
no tiene claves después de la eliminación, se actualiza la raíz del árbol.
*/

void BPlusTree::merge_with_sibling(BPlusNode* node, int idx) {
    BPlusNode* parent = node->parent;
    BPlusNode* sibling = parent->children[idx];
    if (idx < parent->num_keys && parent->children[idx + 1]) {
        BPlusNode* right_sibling = parent->children[idx + 1];
        sibling->keys[sibling->num_keys] = parent->keys[idx];
        sibling->values[sibling->num_keys] = right_sibling->values[0];
        sibling->num_keys++;
        for (int i = 0; i < right_sibling->num_keys; i++) {
            sibling->keys[sibling->num_keys] = right_sibling->keys[i];
            sibling->values[sibling->num_keys] = right_sibling->values[i + 1];
            sibling->num_keys++;
        }
        sibling->next = right_sibling->next;
        if (right_sibling->next) {
            right_sibling->next->prev = sibling;
        }
        delete_child(parent, idx + 1);
        delete right_sibling;
    } else if (idx > 0 && parent->children[idx - 1]) {
        BPlusNode* left_sibling = parent->children[idx - 1];
        left_sibling->keys[left_sibling->num_keys] = parent->keys[idx - 1];
        left_sibling->values[left_sibling->num_keys] = node->values[0];
        left_sibling->num_keys++;
        for (int i = 0; i < node->num_keys; i++) {
            left_sibling->keys[left_sibling->num_keys] = node->keys[i];
            left_sibling->values[left_sibling->num_keys] = node->values[i + 1];
            left_sibling->num_keys++;
        }
        left_sibling->next = node->next;
        if (node->next) {
            node->next->prev = left_sibling;
        }
        delete_child(parent, idx);
        delete node;
    }
}
/*
Esta función fusiona un nodo con uno de sus broders, dependiendo de si el broder es el derecho o el 
izquierdo. Luego, elimina la clave del padre correspondiente al broder que se fusionó y ajusta los 
punteros de los otros hijos (algo así como daddy issues). Si el nodo fusionado es la raíz del árbol, 
se actualiza la raíz del árbol.
*/

void BPlusTree::borrow_from_sibling(BPlusNode* node, int idx) {
    BPlusNode* parent = node->parent;
    BPlusNode* sibling = parent->children[idx];
    if (idx < parent->num_keys && parent->children[idx + 1]) {
        BPlusNode* right_sibling = parent->children[idx + 1];
        node->keys[node->num_keys] = parent->keys[idx];
        node->values[node->num_keys] = right_sibling->values[0];
        node->num_keys++;
        right_sibling->values[0] = right_sibling->values[1];
        for (int i = 0; i < right_sibling->num_keys - 1; i++) {
            right_sibling->keys[i] = right_sibling->keys[i + 1];
            right_sibling->values[i + 1] = right_sibling->values[i + 2];
        }
        right_sibling->num_keys--;
        parent->keys[idx] = right_sibling->keys[0];
    } else if (idx > 0 && parent->children[idx - 1]) {
        BPlusNode* left_sibling = parent->children[idx - 1];
        for (int i = node->num_keys; i > 0; i--) {
            node->keys[i] = node->keys[i - 1];
            node->values[i] = node->values[i - 1];
        }
        node->num_keys++;
        node->values[0] = node->values[1];
        node->keys[0] = parent->keys[idx - 1];
        parent->keys[idx - 1] = left_sibling->keys[left_sibling->num_keys - 1];
        node->values[0] = left_sibling->values[left_sibling->num_keys];
        left_sibling->num_keys--;
    }
}
/*
Esta función pide "prestada" digamos una clave a uno de los hermanos del nodo, dependiendo de si 
el BRODER es el derecho o el izquierdo. Luego, ajusta las claves y los punteros correspondientes 
en el nodo y en el BRODER. Si el nodo prestado es la raíz del árbol, se actualiza la raíz del árbol.
*/

BPlusNode* BPlusTree::search_node(int key) {
    BPlusNode* curr_node = root;
    while (!curr_node->is_leaf) {
        int i = 0;
        while (i < curr_node->num_keys && key >= curr_node->keys[i]) {
            i++;
        }
        curr_node = curr_node->children[i];
    }
    return curr_node;
}
/*
Esta función comienza en la raíz del árbol y busca el nodo hoja que debería contener la clave 
especificada. En cada iteración, se encuentra el hijo apropiado del nodo actual para continuar 
la búsqueda, hasta que se llega a un nodo hoja. Si la clave no está en el árbol, esta función 
devolverá el nodo hoja más cercano en el que se debería haber insertado la clave.
*/


void BPlusTree::print(BPlusNode* node, int level) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < node->num_keys; i++) {
        if (node->is_leaf) {
            cout << " (" << node->keys[i] << ", " << node->values[i] << ")";
        } else {
            print(node->children[i], level + 1);
            cout << endl << setw(level * 4) << " ";
            cout << node->keys[i];
        }
    }
    if (!node->is_leaf) {
        print(node->children[node->num_keys], level + 1);
    }
}
/*
Esta función imprime los contenidos del árbol de forma recursiva, comenzando en el nodo especificado 
y continuando hacia abajo en el árbol. Para cada nodo, la función imprime sus claves y, si el nodo 
no es una hoja, llama a sí misma recursivamente para imprimir los hijos del nodo. La variable level 
se utiliza para asegurarse de que la salida se imprima con la sangría correcta para cada nivel del 
árbol. Fue la mas facil :rip:
*/


void BPlusTree::delete_child(BPlusNode* node, int idx) {
    // Eliminamos el hijo en el índice idx
    delete node->children[idx];
    // Movemos los hijos restantes para llenar el espacio dejado por el hijo eliminado
    for (int i = idx; i < node->num_keys; i++) {
        node->children[i] = node->children[i+1];
    }
    // Movemos las claves restantes una posición hacia atrás
    for (int i = idx; i < node->num_keys-1; i++) {
        node->keys[i] = node->keys[i+1];
    }
    // Reducimos el número de claves
    node->num_keys--;
}
/*
Esta función se utiliza para eliminar un hijo y su clave asociada de un nodo no hoja. 
La función toma como argumentos el nodo y el índice del hijo que se va a eliminar. 
Primero, la función elimina el hijo en el índice idx y libera la memoria correspondiente.
Nota: Malditos hindues q no enseñan bien como liberar la memoria foda sera. 
Luego, la función mueve los hijos restantes y las claves hacia atrás para llenar el espacio dejado 
por el hijo eliminado. Por último, la función reduce el número de claves del nodo en uno.
*/