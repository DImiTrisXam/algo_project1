#ifndef __LSHSEARCH__
#define __LSHSEARCH__

#include "../utilities/PriorityQueue.hpp"
#include "../utilities/grid.hpp"
#include "../utilities/hash.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable **, int, const std::function<double(const Data &, const Data &)> &);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable **, Grid **, int, std::string &, const std::function<double(const Data &, const Data &)> &);
std::vector<std::string> approximateRangeSearch(Data &, double, HashTable **, int, const std::function<double(const Data &, const Data &)> &);

#endif
