#pragma once
#include <string>
#include <cstdint>
#include <sstream>

typedef uint64_t uuid_64; 

struct Transaction
{
    uuid_64 recipient;
    uuid_64 sender;
    int64_t momentoftransact; // unix epoch of transaction
    uint64_t qty; // la cantidad es entera para evitar problemas de redondeo de floats. 
                  // si quieren decimales, interpreten el qty como el numero de centavos (unidad indivisible mas pequenha)
                  // de la moneda en cuestion.
};

std::string to_string(const Transaction& tojsonify)
{
    // should return a valid json representation of the object
    std::ostringstream os;

    os << "{";
    os << "\"recipient\":\"0x" << tojsonify.recipient <<"\",";
    os << "\"sender\":\"0x" << tojsonify.sender <<"\",";
    os << "\"time\":\"0x" << tojsonify.momentoftransact <<"\",";
    os << "\"ammount\":\"0x" << tojsonify.qty << "\"";
    os << "}";

    return os.str();
}