#include "hashing/hashpp.h"
#include <sstream>
#include <iomanip>

#include "uint512.h"
#include "transaction.h"

#define DEPOSIT_FROM_NOTHING_ACCOUNT 0x0000000000000000
#define BLOCK_SIZE 10
#define HASH_CHAR_SIZE 64

typedef uint64_t uuid_64; 

struct Block
{
    uuid_64 id;
    uint512_t prev; // sha512 of previous block
    Transaction transactions[BLOCK_SIZE]; // transaction data
    uint512_t salt; //for proof of work: basicamente alterar esto para hacer que el hash resultante cumpla con alguna caracteristica particular: ej: que el primer digito sea 0.


    // wit a reference to the previous block, compute the appropiate hash for that one
};

std::string to_string(const Block& tojsonify)
{
    // should return a valid json representation of the object
    std::ostringstream os;
    os << "{" ;
    // id
    os << "\"id\":" << tojsonify.id << "," ;
    // prev
    os << "\"prev\":\"" << to_string(tojsonify.prev) << "\"," ;
    // transactions
    os << "\"transactions\":[";
    for (int i = 0; i < BLOCK_SIZE; i++)
    {                                                  // la coma del final
        os << to_string(tojsonify.transactions[i]) << (i == BLOCK_SIZE - 1? "":",");
    }
    os <<"],";
    // salt
    os << "\"salt\":\"" << to_string(tojsonify.salt) << "\"}";

    return os.str();
}  

uint512_t hashBlock(const Block& to_hash)
{
    uint512_t real_hash;
    hashpp::hash hsh_obj = hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_512,to_string(to_hash));
    real_hash = getui512fromstr(hsh_obj.getString());
    
    return real_hash;
}

uint512_t hashBlock(Block&& to_hash)
{
    uint512_t real_hash;
    hashpp::hash hsh_obj = hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_512,to_string(to_hash));
    real_hash = getui512fromstr(hsh_obj.getString());
    
    return real_hash;
}

Block readFromFile()
{
    // read the json formatted string and create an object with the appropiate data
    return Block();
}

/*

Unas cuantas convenciones:
     La cuenta que va a figurar como sender cuando haya alguna clase de deposito va a ser la 0x0000000000000000
     El momento de la transaccion sera la unix epoch, o segundos transcurridos desde enero 1 de 1970. Hay un monton de funciones que sacan eso

*/