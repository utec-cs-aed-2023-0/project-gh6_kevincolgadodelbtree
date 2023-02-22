#include <string>
#include <fstream>
#include "block.h"

#define BLOCKCHAIN_FILE_PREFIX std::string("o")

/*
- class Blockchain
    - void doTransaction(Transaction trs) : push the transaction which leads to push a new block.
    - Block getLastBlock()                : # TODO
    - void push_Block(Block& block)       : Save the block to a .json (called from doTransaction).
    - void PoWGenerateHash()              : Increase salt until the hash satisfies the POW condition.
    - bool validateHash(uint512_t hash)   : Returns if the Hash satisfies the condition.
*/

class Blockchain
{
    int difficulty;
    int nregs;
    uuid_64 lastblock;
    Block UncommitedBlock;
    int transactNo;

public:
    Blockchain(){
        difficulty = 4;
        nregs = 0;
    
        Block temp;
        temp.id = nregs;
        temp.prev = 0;
        lastblock = NULL;
        UncommitedBlock = temp;
        transactNo = 0;
    }

    Blockchain(std::string mdata)
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
        if (transactNo == BLOCK_SIZE) // if block contains the max number of transaccions 
        {                           // which means that the block is full...
            std::cout << "\n [INFO] Commiting Block... " << UncommitedBlock.id << ": \n";
            std::cout << " [INFO] Mining..." << std::endl;

            PoWGenerateHash(); // generate NONCE
            push_Block(UncommitedBlock); // push the block
            std::cout << " [INFO] Block Mined Succesfully. Cleaning Up." << std::endl;

            UncommitedBlock.prev = hashBlock(UncommitedBlock);
            UncommitedBlock.salt = 0;
            lastblock = UncommitedBlock.id;
            nregs++; // increase id
            UncommitedBlock.id = nregs;
            transactNo = 0; // now we need a new block and transactions are now 0 again
            std::cout << " [INFO] Updating metadata..." << std::endl;

            // after updating the block, update the metadata
            push_metadata();
            std::cout << " [INFO] Cleanup finished." << std::endl;

        }
        UncommitedBlock.transactions[transactNo] = trs; // else push the transaction
        transactNo++;
        std::cout << "Transaction added to Uncommited." <<std::endl;
    }

    Block getLastBlock()
    {
        return Block();
    }

private:

    void push_metadata()
    {
        // jsonify and push the metadata of the blockchain
        std::ofstream mdtafile("jsons/blockchain-metadata.json");
        mdtafile << "{";
        mdtafile << "\"difficulty\" :" << difficulty << ",";
        mdtafile << "\"block_qty\" :" << nregs << ",";
        mdtafile << "\"last_block\" : " << lastblock;
        mdtafile << "}";
        mdtafile.close();

    }

    void push_Block(Block& block)
    {
         // the push is just to create the JSON file
        saveJSON(block);                                               
    }

    void PoWGenerateHash()
    {
        uint512_t hash = hashBlock(UncommitedBlock);
        
        while(!validateHash(hash)){ // increase nonce until hash starts with 0x000(difficulty)
            UncommitedBlock.salt++; 
            hash = hashBlock(UncommitedBlock);
        }
        std::cout << "Hash validated with a Nonce of: " << UncommitedBlock.salt << std::endl; // if found, cout nonce
    }

    bool validateHash(uint512_t hash){
        std::string h = to_string(hash);
        for(int i = 2; i < difficulty+2; i++){ // start in 0x[ here ] 
            if(h[i] != '0'){ // prev did an inf loop bc of comparing it to 0 instead of '0'
                return false;
            }
        }
        std::cout << "Hash: " << h << std::endl; // if found, cout hash
        return true;
    }
};