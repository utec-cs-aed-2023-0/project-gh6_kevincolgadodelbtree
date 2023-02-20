#include <iostream>
#include "hashing/hashpp.h"
#include "blockchain.h"

void test(){ // Test que hizo JD, lo dejo xsiaca
    Block test;
    for (int i = 0; i<BLOCK_SIZE;i++)
        test.transactions[i] = Transaction{(uint64_t)i,0,0,0};
    std::cout << to_string(test) << std::endl;
    std::cout << to_string(hashBlock(test)) << std::endl;
}

void test1(){ // Test que hizo Martin
    Blockchain chain;
    for(int i = 0; i < 131; i++){
        chain.doTransaction(Transaction{(uint64_t)i,0,0,0});
    }
}

int main(void)
{
    test1();   
}