#ifndef NODE_H
#define NODE_H

template<typename T>
struct NodeAVL{
    T data;
    int height;
    NodeAVL* left;
    NodeAVL* right;

    //Default constructor
    NodeAVL() {}
    //Constructor con 3 parametros (Data,L,R)
    NodeAVL(T data, NodeAVL *left, NodeAVL *right) : data(data), left(left), right(right), height(0) {}
    //Constructor con 4 parametros (Data,L,R,Height)
    NodeAVL(T data, int height, NodeAVL *left, NodeAVL *right) : data(data), height(height), left(left), right(right) {}

    void KillSelf(){
        if(this->left != nullptr){
            this->left->KillSelf();
        }
        if(this->right != nullptr){
            this->right->KillSelf();
        }
        delete this;
    }
};
#endif