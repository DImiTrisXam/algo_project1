#ifndef __METRICS__
#define __METRICS__

#include <vector>

double euclidianDist(const std::vector<float> &, const std::vector<float> &);
int manhattanDist(const std::vector<float> &, const std::vector<float> &);
int hammingDist(int, int);

#endif
