#include <iostream>

#include "hashing/hashpp.h"
#include "block.h"

int main(void)
{

    Block test;

    for (int i = 0; i<BLOCK_SIZE;i++)
        test.transactions[i] = Transaction{(uint64_t)i,0,0,0};
    test.salt = test.prev = 0;
    std::cout << to_string(test) << std::endl;
    std::cout << to_string(hashBlock(test)) << std::endl;
    
}
