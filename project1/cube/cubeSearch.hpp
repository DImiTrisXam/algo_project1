#ifndef __CUBESEARCH__
#define __CUBESEARCH__

#include "../utilities/hypercube.hpp"
#include "../utilities/utilities.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable *, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable *, int, int, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
std::vector<std::string> approximateRangeSearch(Data &, int, HashTable *, int, int, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);

#endif
