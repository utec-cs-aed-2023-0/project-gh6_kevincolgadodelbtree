/*
template<typename T>
class DynamicArray
{
    int size;
    int capacity;
    T* array;

    DynamicArray(int cap = 10):capacity(cap)
    {
        array = new array[capacity];
    }

    void push_front(T data)
    {

    }

    T pop_back()
    {
        T popped;
        size--;

        return popped;
    }
};

// left = 2*i+1
// right = 2*i+2
// parent = (i - 1)/2 : integer division
// Propiedad del heap: el padre es mayor (maxheap) o menor o igual (minheap) que los hijos
// paraheapificar, donde no se cumple lo arreglo con un swap al mayor (que menos cumple!) de sus hijos y me aseguro que eso llegue al final
// O(nlogn): n/2 padres y log(n) posibles swaps;
// para insertar lo pongo en el siguiente al ultimo espacio y luego 
// para eliminar la raiz: swap del primero con el ultimo y hepify down
// para eliminar random: swapear el elemento con el ultimo y:
    // SI elemento era hoja
class Heap
{

};
*/

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <functional>

//#define COMPARE(X,Y) X > Y
#define LEFT_INDICATOR -1
#define THIS_INDICATOR 0
#define RIGHT_INDICATOR 1
#define NULLINDEX -1
#define ENLARGE_FACTOR 10
                    // callable that returns bool
template<typename T>
class Heap
{
private:
    T* elements;
    int capacity;
    int size;
    std::function<bool(T,T)> CFunct;
public:
    Heap(T* _elements, int _n, std::function<bool(T,T)> Compare= [](T a, T b){ return a > b;})
    {
        this->elements = _elements;
        this->capacity = _n;
        this->size = _n;
        CFunct = Compare; // assign the lambda!
        buildHeap();
    }    
    T top() const
    {
        if(size > 0)
            return elements[0];
        else
            throw std::out_of_range("No hay elementos");
    }
    void push(const T& value){
        if (capacity == size)
            resize();
        size++;
        elements[size-1] = value;

        int index = size-1;
        bool swapped = true;
        while (parent(index) != NULLINDEX && swapped)
        {
            swapped = false;                                       // the 0 is its own parent, so the function will infiniloop
            if (!CFunct(elements[parent(index)],elements[index]) && index != 0)
            {
                T old = elements[parent(index)];
                elements[parent(index)] = elements[index];
                elements[index] = old;
                swapped = true;
            }
            index = parent(index);
        }
    }
    T pop()
    {
        if(size == 0)
            throw std::out_of_range("Invalid pop on empty heap.");
        // pop the old thingy and replace it with the end. save the thingy to return it later
        T old = elements[0];
        elements[0] = elements[size - 1];
        size--; // structure consistency

        // reheapify
        buildHeap();
        return old;
    }

    T* getUnderlyingArray()
    {
        return elements;
    }

    int getSize()
    {
        return size;
    }

private:
    void buildHeap()
    {
        int tempsize = size;
        int index_of_first_non_parent = (size - 1)/2 + 1;
        bool swapped = true;
        while (swapped)
        {
            swapped = false;
            for (int i = 0; i < index_of_first_non_parent;i++)
            {
                int max_indication = whoismax(i);

                if (max_indication == LEFT_INDICATOR)
                {
                    // swap left with i
                    T oldleft = elements[left(i)];

                    elements[left(i)] = elements[i];
                    elements[i] = oldleft; 

                    swapped = true;
                }
                else if (max_indication == RIGHT_INDICATOR)
                {
                    // swap right with i
                    T oldright = elements[right(i)];

                    elements[right(i)] = elements[i];
                    elements[i] = oldright; 
                    
                    swapped = true;
                }   

            }

            index_of_first_non_parent /= 2;
            index_of_first_non_parent += 1; //solo para garantizar que sirva
        }
    }

    // the equivalent of nullptr is -1
    int left(int index) const
    {
        return 2*index + 1 < size? 2*index + 1: -1;
    }
    int right(int index) const 
    {
        return 2*index + 2 < size? 2*index + 2: -1;
    }
    int parent(int index) const
    {
        return (index - 1)/2 < size? (index - 1)/2 : -1;
    }

    void resize(){
        T* oldarray = elements;
        T* elements = new T[capacity + ENLARGE_FACTOR];
        memcpy(elements,oldarray,sizeof(T)*size);
    }

    int whoismax(int index) const
    {
        if (left(index) != NULLINDEX && right(index) != NULLINDEX)
        {
            //caso tiene 2 hijos
            if (CFunct(elements[index],elements[left(index)]) && CFunct(elements[index],elements[right(index)]))
            {
                // caso este es el mayor
                return THIS_INDICATOR;
            }
            else if (CFunct(elements[left(index)],elements[right(index)]))
            {
                // caso left es el mayor
                return LEFT_INDICATOR;
            }
            else
            {
                // caso right es el mayor
                return RIGHT_INDICATOR;
            }
        }
        else if (left(index) == NULLINDEX)
        {
            //caso solo hijo derecho
            if (CFunct(elements[right(index)],elements[index]))
                return RIGHT_INDICATOR;
            else
                return THIS_INDICATOR;
        }
        else if (right(index) == NULLINDEX)
        {
            //caso solo hijo izquierdo
            if (CFunct(elements[left(index)],elements[index]))
                return LEFT_INDICATOR;
            else
                return THIS_INDICATOR;
        }
        else
        {
            // caso ningun hijo
            return THIS_INDICATOR;
        }
    }
};