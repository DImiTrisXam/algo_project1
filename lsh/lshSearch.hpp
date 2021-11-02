#ifndef __LSHSEARCH__
#define __LSHSEARCH__

#include "../utilities/hash.hpp"

struct Neighbor {
  std::string id;
  double dist;
};

void trueDistance();
std::vector<Neighbor> approximateKNN(HashTable **);
void approximateRangeSearch();

#endif
