#pragma once

#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>

#include "trie.h"
#include "helpers.h"

using namespace std;

#define ALPHABET_SIZE 26

class Patricia: public Trie{
public:
    class NodeArray{
    public:
        string prefix;
        list<NodeArray*>* children;
        bool end;



        NodeArray(){
            prefix = "";
            children = new list<NodeArray*>[ALPHABET_SIZE];
            end = false;
        }
        NodeArray(string word){
            prefix = word;
            children = new list<NodeArray*>[ALPHABET_SIZE];
            end = false;
        }
    };
private:
    NodeArray* root;
public:
    Patricia(){
        root = new NodeArray();
        root->end = false;
    }

    void insert(string word) override{
        insert(root, word);
    }

    void insert(NodeArray* node, string word){
        //cout << "Inserting Substring: " << word << endl;
        NodeArray* temp = node;
        if(temp->children->empty())
        { 
            // If is empty just push.
            NodeArray* _node = new NodeArray(word);
            _node->end = true;
            // find appropiate position to insert not done because empty
            temp->children->push_back(_node);
            return;
        } 
        else 
        {
            for(auto ptr = temp->children->begin(); ptr != temp->children->end(); ptr++){
                if(boyermoore(word, (*ptr)->prefix)){
                    //cout << "BOYER MOORE" << endl;
                    insert((*ptr), word.substr(int((*ptr)->prefix.size()), word.size()));
                    return;
                }
                // Previous boyer-moore was to see if there was a prefix for the word to insert.
                // Now we check if the word to insert is prefix for some already inserted word.
                if(boyermoore((*ptr)->prefix, word)){
                    // cout << "need to split: " << (*ptr)->prefix << " cuz of " << word << endl;
                    string new_pre = (*ptr)->prefix.substr(int(word.size()), int((*ptr)->prefix.size())); 
                    // new_pre = new splitted prefix
                    NodeArray* new_node = new NodeArray(word); // Instantiate new Node
                    (*ptr)->prefix = new_pre;                  // Change preffix for the new splitted pre.
                    new_node->children->push_back(*ptr);      // Push old node into the new Node
                    temp->children->push_back(new_node);      // Push the new Node
                    temp->children->remove(*ptr);              // Remove old node

                    // i cant be bothered to insert these things ordered, so.... well
                    new_node->children->sort(
                    [](NodeArray* x,NodeArray* y)
                    {
                        return (x->prefix < y->prefix);
                    }
                    );
                    temp->children->sort(
                    [](NodeArray* x,NodeArray* y)
                    {
                        return (x->prefix < y->prefix);
                    }
                    );
                    return;
                } 
            }

            NodeArray* _node = new NodeArray(word);
            _node->end = true;
            // find appropiate position
            // insert ordered
            temp->children->push_back(_node);

            temp->children->sort(
            [](NodeArray* x,NodeArray* y)
            {
                return (x->prefix < y->prefix);
            }
            );

            temp->end = true;
            //cout << "inserting at root: " << word << endl;
            return;
        }
    }

    string toString(string sep = "") override {
        string buff = "";
        words(root, "", "", buff, sep);
        return buff.substr(1,(int)buff.size());
    }

    void remove(string key) override
    {
        // stack things up
        string word = key;
        NodeArray* node = root;

        stack<NodeArray*> st;

        while (word != "" && node != nullptr)
        {
            for (auto i: *(node->children))
            {
                if (isPrefix(i->prefix,word))
                {
                    st.push(i);
                    node = i;

                    // reduce the word
                    word = word.substr(i->prefix.size());

                    break;
                }
            }
        }
        // now i have a traceback
        // allways remove the end qualifier
        st.top()->end = false;
        // now, delete all elements from stack until a branch is found, an end == true element is found or is root.
        // cases where other words may require the thingy
            // just 1 remaining       only 0 or 1 children means no subtries an end means another word prefixes the one being deleted
        while((st.top() != root) && (st.top()->children->size() < 2) && !st.top()->end)
        {
            NodeArray* temp = st.top(); // keep this to eventually delete it
            delete st.top();
            st.pop();
            st.top()->children->remove(temp);
        }
    }

    /*
    void remove(string word) {
        remove(root, word, 0);
    }
    */

    bool search(string word) override {
        bool found = searchRec(root, word);
        //cout << "Searching: " << word << ": ";
        //if(found) cout << "true\n";
        //else cout << "false\n";        
        return found;
    }

private:

    bool isPrefix(string prf,string wrd)
    {

        if (prf.size() > wrd.size()) return false; // prefixes cant be bigger than the word itself

        // check that all letters are equal
        for (int i = 0; i<prf.size(); i++)
        {
            if (prf[i] != wrd[i]) return false;
        }

        return true;
    }

    bool numChilds(NodeArray* node){
        int c = 0;
        NodeArray* temp = node;
        for(auto ptr = temp->children->begin(); ptr != temp->children->end(); ptr++){
            c++;
        }
        return c;
    }

    void display() override
    {
        displayRec(root, "root");
        cout << endl;
    }

    int bucket_size(NodeArray* node){
        int elements = 0;
        for(auto ptr = node->children->begin(); ptr != node->children->end(); ptr++){
            elements++;
        }
        return elements;
    }

    // Print hash buckets
    void displayFull(NodeArray* node){
        int b_size = bucket_size(node);
        for(auto ptr = node->children->begin(); ptr != node->children->end(); ptr++){
            if(b_size <= 1){ 
                if((*ptr)->end) cout << "!";
                cout << "[ " << (*ptr)->prefix << " ]";
            } else {
                if((*ptr)->end) cout << "!";
                cout << "[ " << (*ptr)->prefix << " ] - ";
            }
            b_size--;
        }
    }

    // Go recursively through the trie.
    void displayRec(NodeArray* node, string prefix){
        if(node->children->empty()) return;
        cout << endl << "child of " << prefix << " : ";
        displayFull(node);
        for(auto ptr = node->children->begin(); ptr != node->children->end(); ptr++){
            displayRec((*ptr), (*ptr)->prefix);
        }
    }

    void words(NodeArray* node, string val, string stack,string& buffer,string sep = " ", bool final = false){
        stack += val;
        if (node->end)
            buffer += stack + sep;
        for(auto ptr = node->children->begin(); ptr != node->children->end(); ptr++){
            bool _end = false; if((*ptr)->end) _end = true;
            words((*ptr), (*ptr)->prefix, stack,buffer, sep, _end);
        }
    }

    bool searchRec(NodeArray* node, string wrd){
        if(wrd == "") return true;
        //cout << " > " << wrd << endl;
        for(auto ptr = node->children->begin(); ptr != node->children->end(); ptr++){
            if(boyermoore(wrd, (*ptr)->prefix)){
                //cout << "boyer: " << wrd << " " << (*ptr)->prefix << endl;
                return searchRec((*ptr), wrd.substr(int((*ptr)->prefix.size()), wrd.size()));
                break;
            }
        }
        return false;
    }
    /*
    bool is_leaf(NodeArray* node) {
        for (auto child : node->children) {
            if (child) {
                return false;
            }
        }
        return true;
    }

    void remove(NodeArray* node, string& word, int index) {
        if (index == word.length()) {
            node->end = false;
            node->word = "";
            return;
        }

        char c = word[index];
        if (node->children[c - 'a']) {
            remove(node->children[c - 'a'], word, index + 1);

            if (!node->children[c - 'a']->end && is_leaf(node->children[c - 'a'])) {
                delete node->children[c - 'a'];
                node->children[c - 'a'] = NULL;
            }
        }
    }
    */
    ~Patricia()
    {

    }
};
