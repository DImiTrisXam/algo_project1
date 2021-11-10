#ifndef __HYPERCUBE__
#define __HYPERCUBE__

#include "hash.hpp"
#include <unordered_map>

class Hypercube : public HashTable {
public:
    Hypercube(int k, int w, int pSize, unsigned int tableSize = 509);
    virtual void add(const std::vector<float> &vec, const std::string &);
    std::list<Data *> &getNeighborCandidates(Data &query, size_t &);

private:
    //std::unordered_map<int, int> vertices;   // maps a bitstring stored as an integer to a bucket(vertice)
    std::list<std::unordered_map<size_t, short>> hValues;
    short f(size_t index, std::unordered_map<size_t, short> &hMap);
    size_t hash(const std::vector<float> &vec);
};


#endif
