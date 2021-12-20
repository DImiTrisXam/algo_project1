#ifndef __GRID__
#define __GRID__

#include "curve_.hpp"

class Grid {
  double delta;
  double t;

public:
  int snapTo1DGrid(Curve_ &);
  int snapTo2DGrid(Curve_ &);
  Grid(double);
};

#endif