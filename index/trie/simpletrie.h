#pragma once

#include <iostream>
#include <string>
#include <forward_list>
#include <stack>
#include <vector>

#include "trie.h"

using namespace std;

#define ALPHABET_SIZE 26
#define ascii_size 256

bool boyermoore(string text, string pattern){
    if(text == pattern) return true;
    int table[ascii_size];
    int size = text.size();

    if(int(pattern.size()) == 1){
        for(int i = 0; i < int(text.size()); i++){
            if(text[i] == pattern[0]){
                return true;
            }
        }
    }

    for(int i = 0; i < ascii_size; i++) table[i] = -1;
    for(int i = 0; i < size; i++) table[int(text[i])] = i;
    // ---------------
    int txt_size = text.size();
    int patt_size = pattern.size();
    int pos = 0;
    //cout << "txt-patt " << txt_size - patt_size << endl;
    while(pos < (txt_size - patt_size)){
        //cout << "pos: " << pos << endl;
        int x = patt_size - 1;
        //cout << "base x: " << x << endl;
        while(x >= 0 && (int(pattern[x]) == int(text[pos + x]))){
            x--;
        }
        //cout << pattern[x] << " & " << text[pos + x] << "|| ";
        //if(pattern[x] == text[pos + x]) cout << "true" << endl;
        //else cout << "false" << endl;

        if(x >= 0){
            unsigned char ch = text[pos + x];
            pos += max(1, x - table[ch]);
        } else {
            if(pos == 0) return true;
            //cout << "pushing in: " << pos << " jumping to: " << pos + patt_size << " x: " << x << endl;
            pos += (pos + patt_size < txt_size-1)? patt_size : 1; 
        }
    }
    //cout << "stopped at: " << pos << endl;
    return false;
}

class SimpleTrie : public Trie{
public:
    class NodeArray{
    public:
        NodeArray *children[ALPHABET_SIZE];
        bool end;

        NodeArray()
        {
            for(int i = 0; i < ALPHABET_SIZE; i++)
            {
                children[i] = nullptr;
            }
            end = false;
        }

        void infanticide()
        {
            // kill and deallocate recursively ALL CHILDREN
            for (int i = 0; i<ALPHABET_SIZE; i++)
            {
                children[i]->infanticide();
                delete children[i];
                children[i] = nullptr;
            }
        }
    };
private:
    NodeArray* root;
public:
    SimpleTrie(){
        root = new NodeArray;
        root->end = false;
    }

    void insert(string key) override
    {
        NodeArray* temp = root;
        for(int i = 0; i < int(key.size()); i++){
            char x = key[i];
            if(temp->children[x - 'a'] == nullptr){
                temp->children[x - 'a'] = new NodeArray;
            }
            temp = temp->children[x - 'a'];
        }
        temp->end = true;
    }

    void remove(string key) override
    {
        //cout << "Erasing: " << w << "..." << endl;
        string word = key;
        stack<NodeArray*> stack;
        stack.push(root);
        
        while(!emptyArr(stack.top()) && word != ""){
            char x = word.front();

            // si el hijo correspondiente al char es valido, actualiza el nodo y pushealo
            if (stack.top()->children[x -'a'] != nullptr)
                stack.push(stack.top()->children[x - 'a']);
            else
                // la palabra no existe, lanza un error
                throw std::out_of_range("Inavlid argument for remove: Trie does not contain word");

            /*
            for(int i = 0; i < ALPHABET_SIZE; i++){
                if(i == x - 'a' && node->children[i] != nullptr){
                    node = node->children[i];
                    stack.push(node);
                    break;
                }
            }
            */
            word = word.substr(1, int(word.size()));
        }

        // TODO: potentially add a guard against empty stack state 
        if (stack.empty())
        {
            //the data structure has reached an invalid state
        }
        else if (stack.top() == root)
        {
            // do NOT delete the root
            return;
        }

        // start actual deletion

        // necesarilly get rid of the endword
        stack.top()->end = false;

        // now, if i am at the end of an orphan branch, lets delete stuff
        if (numChilds(stack.top()) == 0)
        {
            NodeArray* temp = stack.top(); // last deleted INVALID pointer
                    // avoid deleting the root
            while (stack.top() != root && numChilds(stack.top()) <= 1) // while im on the orphan branch
                                                // <= because might be the start of deletion
            {
                temp = stack.top();
                delete stack.top();
                stack.pop();
            }
            // i have ended the deletion of the orphan branch
            // the only allowed invalid pointers are nullptrs, find the invalid one and nullptr it up!
            for (int i = 0; i<ALPHABET_SIZE; i++)
            {
                if (stack.top()->children[i] == temp)
                {
                    stack.top()->children[i] = nullptr;
                    break;
                }
            }
        }
    }

    string toString(string sep = " ") override
    {
        string buf = "";
        words(root, ' ', "", buf, sep);
        return buf;
    }

    bool search(string word) override
    {
        bool found = searchRec(root, word);
        //cout << "Searching: " << word << ": ";
        //if(found) cout << "true\n";
        //else cout << "false\n";
        return found;
    }

private: // non-standard trie functions

    int numChilds(NodeArray* node){
        int c = 0;
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(node->children[i] != nullptr){
                c++;
            }
        }
        return c;
    }

    bool emptyArr(NodeArray* node){
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(node->children[i] != nullptr){
                return false;
            }
        }
        return true;
    }

    void display() override
    {
        cout << "-- TRIE --";
        displayRec(root, '&');
        cout << endl << endl;
    }

    void displayFull(NodeArray* node){
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(node->children[i] != nullptr){
                if(node->children[i]->end) cout << char('a' + i) << "> ";
                else cout << char('a' + i) << " ";
            } else cout << ". ";
        }
    }

    void displayRec(NodeArray* node, char val){
        cout << endl << "child of " << val << " : ";
        displayFull(node);
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(node->children[i] != nullptr){
                displayRec(node->children[i], char('a' + i));
            }
        }
    }

    void words(NodeArray* node, char val, string stack, string& buffer, string sep = " ", bool final = false){
        if(val != ' ') stack += val;
        if(final) buffer += (stack + sep);
        
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(node->children[i] != nullptr){
                bool _end = false;
                if(node->children[i]->end) _end = true;
                
                words(node->children[i], char('a' + i), stack, buffer,sep, _end);
            }
        }
    }

    bool searchRec(NodeArray* node, string wrd){
        if(wrd == "" && node->end) return true;
        char x = wrd.front();
        for(int i = 0; i < ALPHABET_SIZE; i++){
            if(i == x - 'a' && node->children[i] != nullptr){
                return searchRec(node->children[i], wrd.substr(1, wrd.size()));
                break;
            }
        }
        return false;
    }

    ~SimpleTrie()
    {
        root->infanticide();
        delete root;
    }
};