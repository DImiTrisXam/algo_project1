#ifndef __LSHSEARCH__
#define __LSHSEARCH__

#include "../utilities/HashTable.hpp"

// struct Neighbor {
//   std::string id;
//   double dist;
// };

void trueDistance();
std::vector<double> approximateKNN(HashTable **);
void approximateRangeSearch();

#endif
