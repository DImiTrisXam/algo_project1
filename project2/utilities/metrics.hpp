#ifndef __METRICS__
#define __METRICS__

#include <vector>
#include "curve_.hpp"

double euclidianDist(const Data &, const Data &);
int manhattanDist(const Data &, const Data &);
int hammingDist(int, int);
double discreteFrechetDist(const Data &, const Data &);

#endif
