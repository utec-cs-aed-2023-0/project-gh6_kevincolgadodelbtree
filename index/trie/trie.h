#pragma once

#include <iostream>

using namespace std;

const unsigned ALPHA_SIZE = 26;//tamanio del alfabeto: letras minusculas


// this defines an API to trie and trie-like data structures
class Trie
{

public:
    Trie() {}

    virtual void insert(string key) = 0;

    virtual bool search(string key) = 0;

    virtual void remove(string key) = 0;    
    
    //imprime ordenado
    virtual string toString(string sep) = 0;

    ~Trie() 
    {
        ; // noop
    };

    // testeo y debugging:
    virtual void display() = 0;
};
