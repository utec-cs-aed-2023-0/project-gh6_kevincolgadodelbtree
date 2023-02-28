#include "hashing/hashpp.h"
#include <sstream>
#include <iomanip>
#include "JSON/single_include/nlohmann/json.hpp"

#include "uint512.h"
#include "transaction.h"

#define DEPOSIT_FROM_NOTHING_ACCOUNT 0x0000000000000000
#define BLOCK_SIZE 10
#define HASH_CHAR_SIZE 64
using json = nlohmann::json;

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
    uint32_t salt = 0;   // NONCE for proof of work: basicamente alterar esto para hacer que el hash 
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

void printPrettyjson(std::string mdata){
    std::ifstream i("../"+mdata);
    json data = json::parse(i);
    std::cout<<data.dump(4);
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
    os << "\"salt\":" << tojsonify.salt << "}";

    std::ostringstream s;
    s << tojsonify.id;                                              // Get file name
    std::ofstream o(BLOCKCHAIN_FILE_PREFIX + "block-" + s.str() + ".json"); // o is the output file
    o << std::setw(4) << os.str() << std::endl;                     // Write with indent
    if (o.bad())
    {
        // error in the file saving
        throw std::runtime_error("Error while pushing a Block");
    }
    o.close();
}

Block readBlockFromFile(std::string mdata)
{
    //Pueden hacerlo que retorne Block pointer si desean :)
    std::ifstream i(mdata);
    json data = json::parse(i);

    Block readFF;
    readFF.id = data["id"];
    readFF.prev = getui512fromstr(data["prev"]);
    readFF.salt = data["salt"];
    for (int i = 0; i<data["transactions"].size();i++)
    {
        readFF.transactions[i].recipient = data["transactions"][i]["recipient"];
        readFF.transactions[i].sender = data["transactions"][i]["sender"];
        readFF.transactions[i].momentoftransact = data["transactions"][i]["time"];
        readFF.transactions[i].qty = data["transactions"][i]["ammount"];
    }
    

    return readFF;
}

/*
Unas cuantas convenciones:
    - La cuenta que va a figurar como sender cuando haya alguna clase de deposito va a ser 
      la 0x0000000000000000.
    - El momento de la transaccion sera la unix epoch, o segundos transcurridos desde 
      enero 1 de 1970. Hay un monton de funciones que sacan eso
*/