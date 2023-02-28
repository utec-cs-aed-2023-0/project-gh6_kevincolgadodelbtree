#ifndef AVLTree_H
#define AVLTree_H
#include "node.h"
#include<iostream>
#include<cstring>
#include <sstream>

using namespace std;

template<typename T>
class AVLTree{
private:
    NodeAVL<T>* root;
public:
    AVLTree(): root(nullptr) {}

    //Muestra los elementos en orden aplicando el algoritmo de (InOrder)
    void DisplayInOrderAVLTree(){
        return DisplayInOrderAVLTree(root);
    };

    bool find(T value){
        return find(root, value);
    }

    string getPreOrder(){
        return getPreOrder(root);
    }

    T minValue(){
        return minValue(root);
    }
    T maxValue(){
        return maxValue(root);
    }
    bool isBalanced(){
        return isBalanced(root);
    }
    int size(){
        return size(root);
    }

    void remove(T value){
        remove(root, value);
        recorreYbalancea(root);
    }

    void recorreYbalancea(){
        return recorreYbalancea(root);
    }

    //Obtiene la altura de manera recursiva del nodo en cuestion
    int height(){
        return height(root);
    }

    //Muestra el arbolito bien bonito con un algoritmo Pretty
    void printBT(){
        return printBST("",root,false);
    }

    //Inserta un elemento en el arbolito (Recorre desde la raiz)
    void insert(T value){
        return insert(root, value);
    }

    //Actualiza la altura del nodo en cuestion
    void UpdateHeight(NodeAVL<T>* &node){
        node->height = max(height(node->left), height(node->right)) + 1;
        //La altura del nodo se obtiene de la maxima altura entre del hijo izquierdo (Lesser) y el hijo derecho (Greater)
    }

    //Aplica rotacion hacia la izquierda del nodo que desees (Utilizando P, Q, T como referencia de los nodos hijo y nieto)
    //Se da cuando existe un Balance_Factor con signo (-) (Lesser) Rotation
    void LeftRotation(NodeAVL<T>* &node){
        NodeAVL<T> *p = node; //p es Nodo actual
        NodeAVL<T> *q = p->right; //q es el hijo derecho de p (->)(Greater node child)
        NodeAVL<T> *t = q->left; //t es el hijo izquierdo de q (<-)(Lesser node grandchild)
        q->left = p; //El hijo izquierdo de q se vuelve p (<-) (The lesser child of q is now p)
        p->right = t; //El hijo derecho de p se vuelve t (->) (The greater child of p is now t)
        node = q;
        UpdateHeight(node);
    }

    //Aplica rotacion hacia la derecha del nodo que desees (Utilizando P, Q, T como referencia de los nodos hijo y nieto)
    //Se da cuando existe un Balance_Factor con signo (+) (Greater) Rotation
    void RightRotation(NodeAVL<T>* &node){
        NodeAVL<T> *p = node; //p es Nodo actual
        NodeAVL<T> *q = p->left; //q es el hijo izquierda de p (<-)(Lesser node child)
        NodeAVL<T> *t = q->right; //t es el hijo derecho de q (->)(Greater node grandchild)
        q->right = p; //El hijo derecho de q se vuelve p (->) (The greater child of q is now p)
        p->left = t; //El hijo izquierdo de p se vuelve t (<-) (The lesser child of p is now t)
        node = q;
        UpdateHeight(node);
    }

    //Retorna el factor de balanceo del nodo que desees (Resta de ambas alturas de los hijos)
    int Balance_Factor(NodeAVL<T>* node){
        return (height(node->left)- height(node->right));
        /*
            Si sale + (Esta desbalanceado hacía la izquierda) (<-) (Lesser desbalance)
            Si sale - (Esta desbalanceado hacía la derecha) (->) (Greater desbalance)
            Si sale 0 (Esta balanceado) (No desbalance)
        */
    }

    //Balancea cada nodo que esta desbalanceado, ya sea aplicando rotacion simple o rotacion doble
    void Letsbalance(NodeAVL<T>* &node){
        int nodebalancefactor = Balance_Factor(node);
        if(nodebalancefactor >= 2) { // ¿Esta desbalanceado hacia la izquierda (<-)? ¿Node Lesser desbalance?
            if(Balance_Factor(node->left) <= -1) { //¿Su hijo esta desbalanceado hacia la derecha (->)? ¿Child Greater desbalance?
                LeftRotation(node->left); //Rotacion del hijo hacia la izquierda (<-) (Child Lesser rotation)
            }
            RightRotation(node); //Rotacion del nodo actual hacia la derecha (->) (Node Greater rotation)
        }
        if(nodebalancefactor <= -2) { //¿Esta desbalanceado hacia la derecha (->)? ¿Node Greater desbalance?
            if(Balance_Factor(node->right) >= 1){ // ¿Su hijo esta desbalanceado hacia la izquierda (<-)? ¿Child Lesser desbalance?
                RightRotation(node->right); //Rotacion del hijo hacia la derecha (->) (Child Greater rotation)
            }
            LeftRotation(node); //Rotacion del nodo actual hacia la izquierda (<-) (Node Lesser rotation)
        }
    }

    ~AVLTree(){
        if(root != nullptr){
            root->killSelf();
        }
    }

private:
    void recorreYbalancea(NodeAVL<T>* &root){
        if(root != nullptr){
            UpdateHeight(root);
            Letsbalance(root);
            recorreYbalancea(root->left);
            recorreYbalancea(root->right);
        }
    }

    int height(NodeAVL<T>* node){
        if(node != nullptr) {
            int hl = height(node->left); //Obtiene la altura del subarbol izquierdo (<-) (Lesser)
            int hr = height(node->right); //Obtiene la altura del subarbol derecho (->) (Greater)
            return max(hl, hr) + 1;
        }else{
            return -1; //Si no existe el nodo tiene altura -1
        }
    }

    void remove(NodeAVL<T>* &node, T value){
        if(node == nullptr){
            return;
        }else{
            if (value < node->data) {
                remove(node->left, value);
            } else if (value > node->data) {
                remove(node->right, value);
            }else {
                //Caso 1 => Tiene 0 hijos
                if (node->left == nullptr && node->right == nullptr) {
                    delete node;
                    node = nullptr;
                }
                    //Caso 2 => Tiene 1 hijos (Izq-Der)
                else if (node->left != nullptr && node->right == nullptr) {
                    NodeAVL<T>* temp = node->left;
                    delete node;
                    node = temp;

                } else if (node->left == nullptr && node->right != nullptr) {
                    NodeAVL<T> *temp = node->right;
                    delete node;
                    node = temp;

                }
                    //Caso 3 => Tiene 2 hijos
                else {
                    T temp = maxValue(node->left);
                    node->data = temp;
                    remove(node->left, temp);
                }
            }
        }
    }

    bool isBalanced(NodeAVL<T>* node){
        if (node == nullptr) {
            return true;
        }

        int hl = height(node->left);
        int hr = height(node->right);

        return abs(hl-hr)<=1 && isBalanced(node->left) && isBalanced(node->right);
    }

    int size(NodeAVL<T>* node){
        if(node == nullptr){
            return 0;
        }else{
            return 1 + size(node->left) + size(node->right);
        }
    }

    T minValue(NodeAVL<T>* node){
        if(node == nullptr){
            cout<<"The tree is empty"<<endl;
            throw("Empty tree");
        }else{
            if(node->left != nullptr){
                return minValue(node->left);
            }else{
                return node->data;
            }
        }
    }
    T maxValue(NodeAVL<T>* node){
        if(node == nullptr){
            cout<<"The tree is empty"<<endl;
            throw("Empty tree");
        }else{
            if(node->right != nullptr){
                return maxValue(node->right);
            }else{
                return node->data;
            }
        }
    }


    string getPreOrder(NodeAVL<T>* node) {
        stringstream disp;
        if (node != nullptr) {
            disp << node->data << " ";
            disp << getPreOrder(node->left);
            disp << getPreOrder(node->right);
        }
        return disp.str();

    }


    bool find(NodeAVL<T>*node, T value){
        if(node == nullptr){
            return false;
        }else if(value < node->data){
            return find(node->left, value);
        }else if(value > node->data){
            return find(node->right, value);
        }else {
            return true;
        }
    }

    void DisplayInOrderAVLTree(NodeAVL<T>*node){
        if(node == nullptr){
            return;
        }
        displayorder(node->left);
        cout<<node->data<<" ";
        displayorder(node->right);
    }

    void printBST(const std::string& prefix, const NodeAVL<T>* node, bool isLeft)
    {
        if (node != nullptr)
        {
            cout << prefix;
            cout << (isLeft ? "|--" : "L--");
            // print the value of the node
            cout << node->data << endl;
            // enter the next tree level - left and right branch
            printBST(prefix + (isLeft ? "|   " : "    "), node->right, true);
            printBST(prefix + (isLeft ? "|   " : "    "), node->left, false);
        }
    }

    void insert(NodeAVL<T>*& node, T value){
        if(node == nullptr) {
            node = new NodeAVL<T>();
            node->data = value;
            node->left = nullptr;
            node->right = nullptr;
            node->height = height(node);
        }else if(value < node->data){
            insert(node->left, value);
        }else if(value > node->data){
            insert(node->right, value);
        }else{
            return;
        }
        UpdateHeight(node);
        Letsbalance(node);
    }
};

#endif
