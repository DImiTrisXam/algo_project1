#ifndef __LSHSEARCH__
#define __LSHSEARCH__

#include "../utilities/PriorityQueue.hpp"
#include "../utilities/hash.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable **, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable **, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
std::vector<std::string> approximateRangeSearch(Data &, double, HashTable **, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);

#endif
