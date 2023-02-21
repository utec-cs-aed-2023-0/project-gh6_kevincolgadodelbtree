#include "hashing/hashpp.h"
#include <sstream>
#include <iomanip>

#include "uint512.h"
#include "transaction.h"

#define DEPOSIT_FROM_NOTHING_ACCOUNT 0x0000000000000000
#define BLOCK_SIZE 10
#define HASH_CHAR_SIZE 64

typedef uint64_t uuid_64; 

/*
 ___________________________
|  Block : [ # | 1          ]
|                           |
|  Nonce : [ 811300         ]
|          ,________________|
|  Data  : |{"id": 0,       |
|          | "prev": 0x00   |
|          | "trans": [     |
|          |    ...         |
|          |________________|
|                           |
|  Prev  : [ 00000000000... ]
|  Hash  : [ 0000as44ubc... ]
-----------------------------

- struct Block                              : Blockchain-Block Structure.
- string to_string(const Block& tojsonify)  : Build the JSON but return it as a string.
- uint512_t hashBlock(const Block& to_hash) : Use the SHA256 to hash the Block content.
- void saveJSON(const Block& tojsonify)     : Build the JSON and save it as a .json file.
- Block readFromFile()                      : Create a Block from a file.
*/

struct Block
{
    uuid_64 id;     // BLOCK
    int salt = 0;   // NONCE for proof of work: basicamente alterar esto para hacer que el hash 
                    // resultante cumpla con alguna caracteristica particular: ej: que el primer 
                    // digito sea 0.
    uint512_t prev; // PREV: sha512 of previous block
    Transaction transactions[BLOCK_SIZE]; // Transaction data
};

std::string to_string(const Block& tojsonify)
{
    // should return a valid json representation of the object
    std::ostringstream os;
    os << "{" ;
    // id
    os << "\"id\":" << tojsonify.id << ",\n" ;
    // prev
    os << "\"prev\":\"" << to_string(tojsonify.prev) << "\",\n" ;
    // transactions
    os << "\"transactions\":[";
    for (int i = 0; i < BLOCK_SIZE; i++)
    {                                                  // la coma del final
        os << to_string(tojsonify.transactions[i]) << (i == BLOCK_SIZE - 1? "":",\n");
    }
    os <<"],\n";
    // salt
    os << "\"salt\":\"" << tojsonify.salt << "\"}";

    return os.str();
}

void printPrettyJson(const Block& topretty){
    std::cout<<" ___________________________  ";
    std::cout<<"|  Block : [ # | "<< topretty.id << "          ]";
    std::cout<<"|  Nonce : [ "<<topretty.salt <<"        ]";
    std::cout<<"|          ,________________|";
    std::cout<<"|  Data  : |"<<to_string(topretty)<<"       |";
    std::cout<<"|          |________________|";
    std::cout<<"|  Prev  : [ "<<to_string(topretty.prev)<<" ]\n";
    std::cout<<" ---------------------------- ";
}

// No toquen esta vaina, gracias.
uint512_t hashBlock(const Block& to_hash)
{
    uint512_t real_hash;
    hashpp::hash hsh_obj = hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_512,to_string(to_hash));
    real_hash = getui512fromstr(hsh_obj.getString());
    
    return real_hash;
}
// Tampoco esta vaina, gracias.
uint512_t hashBlock(Block&& to_hash)
{
    uint512_t real_hash;
    hashpp::hash hsh_obj = hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_512,to_string(to_hash));
    real_hash = getui512fromstr(hsh_obj.getString());
    
    return real_hash;
}

void saveJSON(const Block& tojsonify){
    std::ostringstream os;
    os << "{" ;
    // Id
    os << "\"id\":" << tojsonify.id << ",\n" ;
    // Prev
    os << "\"prev\":\"" << to_string(tojsonify.prev) << "\",\n" ;
    // Transactions
    os << "\"transactions\":[";
    for (int i = 0; i < BLOCK_SIZE; i++)
    {                                                  // La coma del final
        os << to_string(tojsonify.transactions[i]) << (i == BLOCK_SIZE - 1? "":",\n");
    }
    os <<"],\n";
    // Salt
    os << "\"salt\":\"" << tojsonify.salt << "\"}";

    std::ostringstream s;
    s << tojsonify.id;                                              // Get file name
    std::ofstream o("jsons/blockchain-block-" + s.str() + ".json"); // o is the output file
    o << std::setw(4) << os.str() << std::endl;                     // Write with indent
    if (o.fail())
    {
        // error in the file saving
        throw std::runtime_error("Error while pushing a Block");
    }
    o.close();
}

Block readFromFile()
{
    // read the json formatted string and create an object with the appropiate data
    return Block();
}

/*
Unas cuantas convenciones:
    - La cuenta que va a figurar como sender cuando haya alguna clase de deposito va a ser 
      la 0x0000000000000000.
    - El momento de la transaccion sera la unix epoch, o segundos transcurridos desde 
      enero 1 de 1970. Hay un monton de funciones que sacan eso
*/