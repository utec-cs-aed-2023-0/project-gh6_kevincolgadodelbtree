[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10088296&assignment_repo_type=AssignmentRepo)

# Blockchain - Project

Implementation done in C++ as our final project for: Algorithms and Data Structures course.  

We used some Bitcoin design principles including SHA256 to hash block information and headers, and mining.

**Requirements**
- Uses C++(idk what version), Hash++ library
 
And unfortunately, everything is stored in memory and is deleted when program quits. We are not using dynamic allocation.

## JSON Library from nlohmann (JSON for Modern C++)

In our Project we use a modern json library that helps us to write, read, merge, convert and a lot of other functions. 
Library from: 
-> https://github.com/nlohmann/json#design-goals 
-> https://json.nlohmann.me/

About this library: 

There are myriads of JSON libraries out there, and each may even have its reason to exist. Our class had these design goals:

Intuitive syntax. In languages such as Python, JSON feels like a first class data type. We used all the operator magic of modern C++ to achieve the same feeling in your code. Check out the examples below and you'll know what I mean.

Trivial integration. Our whole code consists of a single header file json.hpp. That's it. No library, no subproject, no dependencies, no complex build system. The class is written in vanilla C++11. All in all, everything should require no adjustment of your compiler flags or project settings.

Serious testing. Our code is heavily unit-tested and covers 100% of the code, including all exceptional behavior. Furthermore, we checked with Valgrind and the Clang Sanitizers that there are no memory leaks. Google OSS-Fuzz additionally runs fuzz tests against all parsers 24/7, effectively executing billions of tests so far. To maintain high quality, the project is following the Core Infrastructure Initiative (CII) best practices.

Other aspects were not so important to us:

Memory efficiency. Each JSON object has an overhead of one pointer (the maximal size of a union) and one enumeration element (1 byte). The default generalization uses the following C++ data types: std::string for strings, int64_t, uint64_t or double for numbers, std::map for objects, std::vector for arrays, and bool for Booleans. However, you can template the generalized class basic_json to your needs.

Speed. There are certainly faster JSON libraries out there. However, if your goal is to speed up your development by adding JSON support with a single header, then this library is the way to go. If you know how to use a std::vector or std::map, you are already set.

## BlockChain Object
Genesis block is created during initialization.  
The blockchain variables are:
```cpp
int difficulty; // Amount of 0's for the proof-of-work.
int nregs; // Block's ID.
uuid_64 lastblock; // Last Block ID.
Block UncommitedBlock; // Current in-develop Block.
int transactNo; // Amount of transactions on the current Block.
```

### Block Object
```cpp
uuid_64 id; // Self-explantory
int salt = 0; // Nonce
uint512_t prev; // Previous block ID
Transaction transactions[BLOCK_SIZE]; // Array with transactions.
```

#### Transaction Object
```cpp
uuid_64 recipient; // Self-explantory
uuid_64 sender; // Self-explantory
int64_t momentoftransact; // Unix-Epoch of transaction.
int64_t qty; // Money sent (Cents, Dolars, Crypto-Coins, etc).
```

Everytime a block is added to the blockchain it will check that the given hash is valid (With a difficulty of 5, the hash must start with 0x00000...).

#### This repository uses hashing functions from https://github.com/D7EAD/HashPlusPlus.

## Authors

| Martin P. | Johar B.  |  Juan Diego P. | Marcelo Z. | Oscar C. |
|   :---:    |    :----:   |   :---:    |  :---:     |   :---:    |
| <img src="https://avatars.githubusercontent.com/u/87045535?v=4">  | <img src= "https://avatars.githubusercontent.com/u/66704166?v=4">   | <img src="https://avatars.githubusercontent.com/u/66135437?v=4">   | <img src="https://avatars.githubusercontent.com/u/85197213?v=4"> | <img src="https://avatars.githubusercontent.com/u/112034734?v=4">  |
[Martin Perez Bonany](https://github.com/Owzok) |   [Johar Barzola Estrella](https://github.com/Joharjbe)    | [Juan Diego Prochazka](https://github.com/juandiegoproch)  |  [Marcelo Zuloeta](https://github.com/marceloZS)   |  [Oscar Chu Lao](https://github.com/PachoRyan) |
