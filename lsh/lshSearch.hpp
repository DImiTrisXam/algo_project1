#ifndef __LSHSEARCH__
#define __LSHSEARCH__

#include "../utilities/hash.hpp"

struct Neighbor {
  std::string id;
  double dist;
};

std::vector<double> trueDistanceN(Data &, int, HashTable **, int);
std::vector<Neighbor> approximateKNN(HashTable **);
void approximateRangeSearch();

#endif
