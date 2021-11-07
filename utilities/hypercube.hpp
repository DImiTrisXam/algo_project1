#ifndef __HYPERCUBE__
#define __HYPERCUBE__

#include "hash.hpp"

class Hypercube : public HashTable {
public:
    Hypercube(int k, int w, int pSize, unsigned int tableSize = 509);
    void add(const std::vector<float> , const std::string );


private:
    char *bitmap;  //f(h) binary values

};


#endif