#ifndef __METRICS__
#define __METRICS__

#include <vector>
#include "curve.hpp"

double euclidianDist(const std::vector<float> &, const std::vector<float> &);
int manhattanDist(const std::vector<float> &, const std::vector<float> &);
int hammingDist(int, int);
double discreteFrechetDist(const Curve &, const Curve &);
double discreteFrechetDistIter(const Curve &, const Curve &);

#endif
