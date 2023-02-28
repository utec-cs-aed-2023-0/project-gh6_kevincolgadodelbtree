#include <iostream>
#pragma once
using namespace std;

template <typename T>
class Node{
public:
    T value;
    Node *next;
    Node(){
        next = nullptr;
    }

    Node(T data){
        this->value = data;
        this->next = nullptr;
    }
    ~Node(){
        if(next != nullptr)
            delete next;
    }
};  

template <typename T>
class LinkedList{
public:
    class Iterator{
    public:
        Node<T>* current;
        Iterator(Node<T>* ptr){
            current = ptr;
        }    
        Iterator operator++(){
            current = current->next;
            return *this;
        }
        Iterator operator++(int){
            current = current->next;
            return *this;
        }
        T& operator*(){
            return current->value;
        }
        bool operator!=(const Iterator& other) const{
            return current != other.current;
        }
    };
private:
    Node<T>* head;
public:

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }

    Iterator before_begin() const {
        Node<T>* temp = new Node<T>();
        temp->next = head;
        return Iterator(temp);
    }

    Iterator erase_after(Iterator pos){
        Node<T>* temp = pos.current->next->next;
        pos.current->next = temp;
        return Iterator(temp);
    }

    LinkedList(){
        head = nullptr;
    }

    T front(){
        if(head != nullptr) return head->value;
        throw("Empty.");
    }

    T back(){
        if(head == nullptr) throw("List is empty.");

        Node<T>* temp = head;
        while(temp->next != nullptr) temp = temp->next;
        
        return temp->value;
    }

    void push_front(T data){
        Node<T>* newNode = new Node<T>(data);
        if(head == nullptr) head = newNode;
        else{
            newNode->next = head;
            head = newNode;
        }
    }

    void push_back(T data){
        if(head == nullptr) {
            head = new Node<T>(data);
            return;
        }

        Node<T>* temp = head;
        while(temp->next != nullptr) temp = temp->next;
        
        temp->next = new Node<T>(data);
    }

    T pop_front(){
        if(head == nullptr) throw("Empty.");
        Node<T>* prevValue = head;
        head = head->next;
        T data = prevValue->value;
        prevValue->next = nullptr;

        delete prevValue;
        return data;
    }

    // O(n)
    T pop_back(){
        if(head == nullptr) throw("Empty.");
        Node<T>* temp = head;
        while(temp->next->next != nullptr) temp = temp->next;
        Node<T>* nodeToDelete = temp->next;
        T data = nodeToDelete->value;
        temp->next = nullptr;
        
        delete nodeToDelete;
        return data;
    }

    T insert(T value, int pos){
        if(head == nullptr) throw("Empty.");
        if(pos > size()) throw("Index out of range.");
        if(pos == 0) head = new Node<T>(value);

        Node<T>* curNode = head;
        Node<T>* nxtNode = head->next;
        int i = 0;
        Node<T>* newNode = new Node<T>(value);

        while(nxtNode != nullptr){
            if(i == pos-1){
                newNode->next = nxtNode;
                curNode->next = newNode;
                return value;
            }
            curNode = curNode->next;
            nxtNode = nxtNode->next;
            i++;
        }
        return value;
    }

    //igual q en el circular, chequealo martin te convence?
    bool remove(int pos){
        if (pos+1 > this->size() || pos < 0) return false;
        if (pos==0) this->pop_front();
        else if (pos==this->size()-1) this->pop_back();
        else{
            Node<T>* nodo = head;
            for (int i=0; i<pos-1; i++) nodo = nodo->next;
            Node<T> *temp = nodo->next;
            nodo->next = temp->next;
            delete temp;
        }
        return true;
    }

    T& operator[](int index){
        if(index > size()) throw("The given index is out of range.");
        Node<T>* temp = head;
        int i = 0;
        while(temp != nullptr){
            if(i == index)
                return temp->value;
            temp = temp->next;
            i++;          
        }
        return temp->value;
    }

    bool is_empty(){
        if(head == nullptr){
            return true;
        } else {
            return false;
        }
    }

    int size(){
        if(head == nullptr)
            return 0;
        Node<T>* temp = head;
        int i = 0;
        while(temp != nullptr){
            temp = temp->next;
            i++;
        }
        return i;
    }

    void clear(){
        Node<T>* temp = head->next;
        while(temp != nullptr){
            delete head;
            head = temp;
            temp = temp->next;
        }
        head = nullptr;
    }

    bool is_sorted(){
        Node<T>* temp = head;
        while(temp->next != nullptr){
            if(temp->value > temp->next->value)
                return false;
            temp = temp->next;
        }
        return true;
    }

    void sort(){
        Node<T>* currentNode = head; 
        Node<T>* nextNode = nullptr;

        while(currentNode != nullptr){
            nextNode = currentNode->next;
            while(nextNode != nullptr){
                if(currentNode->value > nextNode->value){
                    T temp = currentNode->value;
                    currentNode->value = nextNode->value;
                    nextNode->value = temp;
                }
                nextNode = nextNode->next;
            }
            currentNode = currentNode->next;
        }
    }

    void reverse(){
        Node<T>* currentNode = head;
        Node<T>* nextNode = nullptr, *previousNode = nullptr;

        while(currentNode != nullptr){
            nextNode = currentNode->next;
            currentNode->next = previousNode;
            previousNode = currentNode;
            currentNode = nextNode;
        }
        head = previousNode;
    }

    string to_string(string sep = " "){
        string returnable = "";
        if(is_empty()){
            return "List is empty\n";
        }

        Node<T>* temp = head;
        while(temp != nullptr){
            returnable += std::to_string(temp->value) += sep;
            temp = temp->next;
        }
        returnable += "\n";
        return returnable;
    }
    
    string name(){
        return "Linked List";
    }

    ~LinkedList(){ delete head; }
};