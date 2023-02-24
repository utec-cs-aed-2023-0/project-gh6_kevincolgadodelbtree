#include <string>
#include <fstream>
#include <chrono>
#include "block.h"
#include "JSON/single_include/nlohmann/json.hpp"
using json = nlohmann::json;


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
    uint64_t nregs; //nexts block to be commited
    uuid_64 lastblock; // last commited block
    Block UncommitedBlock;
    int transactNo;

public:

    // commit order iterator stuff

    class commitOrderIterator
    {
        uint64_t id;
        friend Blockchain;

        commitOrderIterator(uuid_64 id_)
        {
            id = id_;
        }

        public:

        commitOrderIterator& operator++()
        {
            id++;
            return *this;
        }
        commitOrderIterator operator++(int)
        {
            commitOrderIterator old = *this;
            id++;
            return old;
        }

        commitOrderIterator& operator--()
        {
            id--;
            return *this;
        }
        commitOrderIterator operator--(int)
        {
            commitOrderIterator old = *this;
            id--;
            return old;
        }

        Block operator*()
        {
            return readBlockFromFile(BLOCKCHAIN_FILE_PREFIX + "block-" + std::to_string(id) + ".json");
        }

        bool operator!=(const commitOrderIterator& other)
        {
            return this->id != other.id;
        }
    };

    commitOrderIterator cobegin()
    {
        return commitOrderIterator(0);
    }

    commitOrderIterator coend()
    {
        return commitOrderIterator(nregs);
    }
public:

    void defaultInit()
    {
        difficulty = 4;
        nregs = 0;
        transactNo = 0;
    
        Block temp;
        temp.id = nregs;
        temp.prev = 0;
        lastblock = 0;
        UncommitedBlock = temp;
        transactNo = 0;
    }

    Blockchain(){
        defaultInit();
    }

    void printPrettyjson(std::string mdata){
        std::ifstream i("../"+mdata);
        json data = json::parse(i);
        std::cout<<data.dump(4);
    }

    Blockchain(std::string mdata)
    {
        // read metadata file and populate the appropiate things
        std::ifstream i(mdata);

        if (!i.good())
        {
            //file doesnt exist. empty blockchain
            defaultInit(); // initialize the object
            push_metadata(); // create the file!
            return;
        }

        json data = json::parse(i);

        difficulty = data["difficulty"];
        nregs = data["block_qty"];
        lastblock = data["last_block"];
        transactNo = 0;

        UncommitedBlock.id = nregs;
        // initialize the uncommited block (TODO)
        //std::ifstream lastblockfile(BLOCKCHAIN_FILE_PREFIX + std::to_string(lastblock));
    }

    void doTransaction(Transaction trs)
    {
        if (transactNo == BLOCK_SIZE) // if block contains the max number of transaccions 
        {                           // which means that the block is full...

            #ifdef DO_INFO_MESSAGES

                std::cout << "\n [INFO] Commiting Block... " << UncommitedBlock.id << ": \n";
                std::cout << " [INFO] Mining..." << std::endl;

            #endif

            PoWGenerateHash(); // generate NONCE
            push_Block(UncommitedBlock); // push the block

            #ifdef DO_INFO_MESSAGES
                std::cout << " [INFO] Block Mined Succesfully. Cleaning Up." << std::endl;
            #endif

            UncommitedBlock.prev = hashBlock(UncommitedBlock);
            UncommitedBlock.salt = 0;
            lastblock = UncommitedBlock.id;
            nregs++; // increase id
            UncommitedBlock.id = nregs;
            transactNo = 0; // now we need a new block and transactions are now 0 again

            #ifdef DO_INFO_MESSAGES
                std::cout << " [INFO] Updating metadata..." << std::endl;
            #endif
            // after updating the block, update the metadata
            push_metadata();

            #ifdef DO_INFO_MESSAGES
                std::cout << " [INFO] Cleanup finished." << std::endl;
            #endif

        }
        #if defined(DO_INFO_MESSAGES) && defined(DO_VERBOSE)
            std::cout << " [INFO - Verbose] Transaction time fetched" <<std::endl;
        #endif
        // transaction time set
        const auto p1 = std::chrono::system_clock::now();
        trs.momentoftransact = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        
        // push transaction
        UncommitedBlock.transactions[transactNo] = trs; // else push the transaction
        transactNo++;

        #if defined(DO_INFO_MESSAGES) && defined(DO_VERBOSE)
            std::cout << " [INFO - Verbose] Transaction added to Uncommited." <<std::endl;
        #endif
    }

    Block getLastBlock()
    {
        readBlockFromFile(BLOCKCHAIN_FILE_PREFIX + "block-" + std::to_string(lastblock) + ".json");
        return Block();
    }

private:

    void push_metadata()
    {
        // jsonify and push the metadata of the blockchain
        std::ofstream mdtafile(BLOCKCHAIN_FILE_PREFIX + "metadata.json");
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
        #if defined(DO_INFO_MESSAGES)
            std::cout << " [INFO] Hash validated with a Nonce of: " << UncommitedBlock.salt << std::endl; // if found, cout nonce
        #endif
    }

    bool validateHash(uint512_t hash)
    {
        std::string h = to_string(hash);
        for(int i = 2; i < difficulty+2; i++){ // start in 0x[ here ] 
            if(h[i] != '0'){ // prev did an inf loop bc of comparing it to 0 instead of '0'
                return false;
            }
        }
        #if defined(DO_INFO_MESSAGES) && defined(DO_VERBOSE)
            std::cout << "[INFO - Verbose] Hash: " << h << std::endl; // if found, cout hash
        #endif
        return true;
    }
};