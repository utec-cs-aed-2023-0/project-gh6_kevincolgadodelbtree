#include "hashing/hashpp.h"
#include <sstream>
#include <iomanip>

#define DEPOSIT_FROM_NOTHING_ACCOUNT 0x0000000000000000
#define BLOCK_SIZE 10
#define HASH_CHAR_SIZE 64

typedef uint64_t uuid_64; 

struct uint512_t
{
    unsigned char data[64];

    uint512_t() = default;

    uint512_t& operator=(uint512_t&& other)
    {
        for (int i = 0; i<64;i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    uint512_t& operator=(unsigned int other)
    {
        char* other_data = (char*)&other;
        // 64 - sizeof(uint) must be 0
        // sizeof(uint) must copy data exactly
        for (int i = 0; i<64; i++)
        {
            data[i] = 0;
        }

        for (int i = 0; i<sizeof(unsigned int); i++)
        {
            data[i] = *other_data;
            other_data++;
        }
        return *this;
    }

    uint512_t(uint512_t&& other)
    {
        for (int i = 0; i<64;i++)
        {
            data[i] = other.data[i];
        }
    }

    uint512_t(const uint512_t& other)
    {
        for (int i = 0; i<64;i++)
        {
            data[i] = other.data[i];
        }
    }
};

int getValueFromHexDigit(char digit)
{
    switch(digit)
    {
        case '0':
        return 0;
        break;
    
        case '1':
        return 1;
        break;

        case '2':
        return 2;
        break;

        case '3':
        return 3;
        break;

        case '4':
        return 4;
        break;

        case '5':
        return 5;
        break;

        case '6':
        return 6;
        break;

        case '7':
        return 7;
        break;

        case '8':
        return 8;
        break;

        case '9':
        return 9;
        break;

        case 'A':
        return 10;
        break;

        case 'a':
        return 10;
        break;

        case 'B':
        return 11;
        break;

        case 'b':
        return 11;
        break;

        case 'C':
        return 12;
        break;

        case 'c':
        return 12;
        break;

        case 'D':
        return 13;
        break;

        case 'd':
        return 13;
        break;

        case 'E':
        return 14;
        break;

        case 'e':
        return 14;
        break;

        case 'F':
        return 15;
        break;

        case 'f':
        return 15;
        break;

        default:
        return -1;
        break;
    }
}

uint512_t getui512fromstr(const std::string& num_string)
{
    uint512_t to_ret;
    unsigned char byte;
    for (int i = 0; i<128;i+=2) // avanzar de 2 en 2, (byte en byte)
    {
        unsigned char msn = getValueFromHexDigit(num_string[i]);
        unsigned char lsn = getValueFromHexDigit(num_string[i+1]);
        byte = msn*16 + lsn;
        
        to_ret.data[i/2] = byte;
    }
    return to_ret;
}

std::string to_string(uint512_t dat)
{
    char hdigits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    std::string to_ret = "0x";

    for (int i=0;i<64;i++) // for byte in 64-byte-long-field
    {
        unsigned int lsn = dat.data[i]%16;
        unsigned int msn = dat.data[i]/16;

        to_ret += hdigits[msn];
        to_ret += hdigits[lsn];
    }

    return to_ret;
}

struct Transaction
{
    uuid_64 recipient;
    uuid_64 sender;
    int64_t momentoftransact; // unix epoch of transaction
    uint64_t qty; // la cantidad es entera para evitar problemas de redondeo de floats. 
                  // si quieren decimales, interpreten el qty como el numero de centavos (unidad indivisible mas pequenha)
                  // de la moneda en cuestion.
};

struct Block
{
    uint512_t prev; // sha512 of previous block
    Transaction transactions[BLOCK_SIZE]; // transaction data
    uint512_t salt; //for proof of work: basicamente alterar esto para hacer que el hash resultante cumpla con alguna caracteristica particular: ej: que el primer digito sea 0.


    // wit a reference to the previous block, compute the appropiate hash for that one
};

std::string to_string(const Transaction& tojsonify)
{
    // should return a valid json representation of the object
    std::ostringstream os;

    os << "{ \n";
    os << "\"recipient\": \"0x" << tojsonify.recipient <<"\", \n";
    os << "\"sender\": \"0x" << tojsonify.sender <<"\", \n";
    os << "\"time\": \"0x" << tojsonify.momentoftransact <<"\", \n";
    os << "\"ammount\": \"0x" << tojsonify.qty << "\" \n";
    os << "}";

    return os.str();
}

std::string to_string(const Block& tojsonify)
{
    // should return a valid json representation of the object
    std::ostringstream os;
    os << "{ \n";
    // prev
    os << "\"prev\" : \"" << to_string(tojsonify.prev) << "\", \n";
    // transactions
    os << "\"transactions\" : [";
    for (int i = 0; i < BLOCK_SIZE; i++)
    {                                                  // la coma del final
        os << to_string(tojsonify.transactions[i]) << (i == BLOCK_SIZE - 1? "":", ");
    }
    os <<"], \n";
    // salt
    os << "\"salt\" : \"" << to_string(tojsonify.salt) << "\" \n }";

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
/*

Unas cuantas convenciones:
     La cuenta que va a figurar como sender cuando haya alguna clase de deposito va a ser la 0x0000000000000000
     El momento de la transaccion sera la unix epoch, o segundos transcurridos desde enero 1 de 1970. Hay un monton de funciones que sacan eso

*/