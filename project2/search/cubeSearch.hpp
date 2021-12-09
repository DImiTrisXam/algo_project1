#ifndef __CUBESEARCH__
#define __CUBESEARCH__

#include "../utilities/hypercube.hpp"
#include "../utilities/utilities.hpp"

std::vector<Neighbor> trueDistanceN(Data &, int, HashTable *, const std::function<double(const Data&, const Data&)> &);
std::vector<Neighbor> approximateKNN(Data &, int, HashTable *, int, int, int, const std::function<double(const Data&, const Data&)> &);
std::vector<std::string> approximateRangeSearch(Data &, int, HashTable *, int, int, int, const std::function<double(const Data&, const Data&)> &);

#endif
