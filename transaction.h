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
        
    uint64_t hash_func(){ return qty + uint64_t(momentoftransact); }

        // ===== MomentOfTransact =====
    bool equal_greater_mot(int64_t other){ return (momentoftransact >= other); }
    bool equal_greater_mot(int64_t other) const { return (momentoftransact >= other); }

    bool equal_less_mot(int64_t other){ return (momentoftransact <= other); }
    bool equal_less_mot(int64_t other) const { return (momentoftransact <= other); }

    bool less_mot(int64_t other){ return (momentoftransact < other); }
    bool less_mot(int64_t other) const { return (momentoftransact < other); }

    bool greater_mot(int64_t other){ return (momentoftransact > other); }
    bool greater_mot(int64_t other) const { return (momentoftransact > other); }
        // ===== QTY =====
    bool equal_greater_qty(uint64_t other){ return (qty >= other); }
    bool equal_greater_qty(uint64_t other) const { return (qty >= other); }

    bool equal_less_qty(uint64_t other){ return (qty <= other); }
    bool equal_less_qty(uint64_t other) const { return (qty <= other); }

    bool less_qty(uint64_t other){ return (qty < other); }
    bool less_qty(uint64_t other) const { return (qty < other); }

    bool greater_qty(uint64_t other){ return (qty > other); }
    bool greater_qty(uint64_t other) const { return (qty > other); }

        // ===== SENDER =====
    bool equals_sender(uuid_64 other){ return ( other == sender ); }
    bool equals_sender(uuid_64 other) const { return ( other == sender ); }

        // ===== RECIPIENT =====
    bool equals_recipient(uuid_64 other){ return ( other == recipient ); }
    bool equals_recipient(uuid_64 other) const { return( other == recipient ); }
};

std::string to_string(const Transaction& tojsonify)
{
    // should return a valid json representation of the object
    std::ostringstream os;

    os << "{";
    os << "\"recipient\":" << tojsonify.recipient <<",";
    os << "\"sender\":" << tojsonify.sender <<",";
    os << "\"time\":" << tojsonify.momentoftransact << ",";
    os << "\"ammount\":" << tojsonify.qty << "";
    os << "}";

    return os.str();
}