// Compile Configuration
#define DO_INFO_MESSAGES
//#define DO_VERBOSE
#define BLOCKCHAIN_FILE_PREFIX std::string("jsons/blockchain-")

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
    Blockchain chain("jsons/blockchain-metadata.json");
    int blocks;
    std::cout << "How many blocks: \n";
    std::cin >> blocks;
    for(int i = 0; i < (blocks*10)+1; i++){
        chain.doTransaction(Transaction{(uint64_t)i,0,0,0});
    }

    for (auto i = chain.cobegin(); i != chain.coend(); i++)
    {
        std::cout << to_string(*i) << std::endl;
    }
}

Transaction createTransaction(uint64_t user){
    uint64_t acc, time, qty;
    std::cout << "\nBeneficiary accound number: ";
    std::cin >> acc;
    std::cout << "\nEnter amount to send: ";
    std::cin >> qty;
    return Transaction{acc, user, 0, qty};
}

void manualTest(){
    uint64_t user;
    std::cout << "Enter your bank account: ";
    std::cin >> user;

    Blockchain chain;

    while(true){
        chain.doTransaction(createTransaction(user));
        std::cout << "Transaction done successfully" << std::endl;
    }
}

int main(void)
{
    std::string option;
    while(option != "manual" || option != "auto"){
        std::cout << "\nEnter: \"manual\" or \"auto\"\n";
        std::cin >> option;
        if(option == "manual"){
            manualTest();
            break;
        } else if(option == "auto"){
            test1();
            break;
        }
    }
}