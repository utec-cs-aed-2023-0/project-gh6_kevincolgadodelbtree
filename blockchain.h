#include <string>
#include <fstream>
#include "block.h"

#define BLOCKCHAIN_FILE_PREFIX std::string("blockchain-block-")

class Blockchain
{
    int difficulty;
    int nregs;
    uuid_64 lastblock;
    Block UncommitedBlock;
    int transactNo;

public:
    BlockChain(std::string mdata)
    {
        // read metadata file and populate the appropiate things
        std::ifstream mdatafile(mdata);
        mdatafile >> difficulty;
        mdatafile >> nregs;
        mdatafile >> lastblock;
        transactNo = 0;
        
        // initialize the uncommited block
        std::ifstream lastblockfile(BLOCKCHAIN_FILE_PREFIX + std::to_string(lastblock));

    }

    void doTransaction(Transaction trs)
    {
        if (transactNo == BLOCK_SIZE)
        {
            push_Block(UncommitedBlock);
            transactNo = 0;
        }
        UncommitedBlock.transactions[transactNo] = trs;
    }

    Block getLastBlock()
    {

    }

private:

    void push_Block()
    {

    }

    void PoWGenerateHash()
    {
        UncommitedBlock.salt = 0;
    }

};