#ifndef __CUBESEARCH__
#define __CUBESEARCH__

#include "../utilities/hash.hpp"
#include "../utilities/PriorityQueue.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable **);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable **);
std::vector<std::string> approximateRangeSearch(Data &, int, HashTable **);

#endif
