#ifndef __LSHSEARCH__
#define __LSHSEARCH__

#include "../utilities/hash.hpp"
#include "../utilities/PriorityQueue.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable **, int);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable **, int);
std::vector<std::string> approximateRangeSearch(Data &, double, HashTable **, int);

#endif
