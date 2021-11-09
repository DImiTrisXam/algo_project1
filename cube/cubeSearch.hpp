#ifndef __CUBESEARCH__
#define __CUBESEARCH__

#include "../utilities/hypercube.hpp"
#include "../utilities/utilities.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable *);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable *);
std::vector<std::string> approximateRangeSearch(Data &, int, HashTable *);

#endif
