#include <cstdint>
#include <stdexcept>

#define HMAP_DEFAULT_CELLS 5
#define HMAP_DEFAULT_MAXLISTLEN 5;

#define HMAP_NCELLS_GROWTH_FACTOR = 5;

template<typename T>
uint64_t hash(const T& data)
{
    // user must provide a good overload for the hash function of the data
    // by default no implementation is provided
    return 0;
}

uint64_t hash(int data)
{
    return (uint64_t)data;
}

template<typename T>
class HashMap
{
private:
    struct LinkedListNode
    {
        T data;
        LinkedListNode next;
        uint64_t data_hash;
    };

private:
    LinkedListNode** array;
    int n_cells;
    int n_elems;

    int max_list_len;
    /*
        El hash admite como maximo listas tamanho max_list_len antes de rehashear. El fill_factor es innecesario
        porque deja de ser relevante como medio para evitar colisiones muy costosas.
    */
public:
    HashMap()
    {
        n_cells = HMAP_DEFAULT_CELLS;
        n_elems = 0;
        max_list_len = 5;

        p_build();
    }

    LinkedListNode& insert(const T& data)
    {
        LinkedListNode* node = new LinkedListNode;
        node->data = data;
        node->hash = hash(data);

        insert_node(node);
    }

    void remove(const T& data)
    {
        int index = hash(data) % n_cells;

        if (array[index] == nullptr)
            throw std::invalid_argument("Element to delete does not exist");
        
        LinkedListNode* prev_to_delete;

        while (prev_to_delete->next != nullptr)
        {
            if (prev_to_delete->next->data == data)
                remove_node(prev_to_delete);
                return;
        }
        throw std::invalid_argument("Element to delete does not exist");
    }

    bool exists(const T& data) const
    {
        int index = hash(data) % n_cells;

        if (array[index] == nullptr)
            return false;
        
        LinkedListNode* prev;

        while (prev->next != nullptr)
        {
            if (prev->next->data == data)
                return true;
        }
        return false;
    }
    
    T& get_reference_to(const T& data)
    {
        int index = hash(data) % n_cells;

        if (array[index] == nullptr)
            throw std::out_of_range("Key does not exist in hash");
        
        LinkedListNode* prev;

        while (prev->next != nullptr)
        {
            if (prev->next->data == data)
                return prev->next->data;
        }
        throw std::out_of_range("Key does not exist in hash");
    }

private:
    void p_build()
    {
        array = new LinkedListNode**[n_cells];
    }

    int insert_node(LinkedListNode* node)
    {
        int index = node->data_hash % n_cells;


        LinkedListNode* prev = array[index];
        LinkedListNode* current = array[index];
        int count = 0;
        while (current != nullptr)
        {
            ++count;
            prev = current;
            current = current->next;
        }

        prev->next = node;
        node->next = nullptr;

        n_elems++;

        return count;
    }

    void remove_node(LinkedListNode* prev)
    {
        LinkedListNode new_next = prev->next->next;
        delete prev->next;
        prev->next = new_next;
        n_elems--;
    }

    void rehash_up()
    {
        int old_n_cells = n_cells;
        n_cells = nextsize(n_cells);

        rehash();
    }

    void rehash_down()
    {
        int old_n_cells = n_cells;
        n_cells = prevsize(n_cells);

        rehash();

    }
    void rehash()
    {

        LinkedListNode** oldarray = array;

        array = new LinkedListNode*[n_cells];
        n_elems = 0;

        for (int i = 0; i<old_n_cells;i++)
        {
            LinkedListNode* current = oldarray[i];
            while (current != nullptr)
            {
                LinkedListNode* next = current->next;
                insert_node(current);
                current = next;
            }
        }
        
        delete[] oldarray;
    }
private:
    unsigned int nextsize(int current_size)
    {
        return current_size + HMAP_NCELLS_GROWTH_FACTOR;
    }

    unsigned int prevsize(int current_size)
    {
        return current_size - HMAP_NCELLS_GROWTH_FACTOR;
    }
};