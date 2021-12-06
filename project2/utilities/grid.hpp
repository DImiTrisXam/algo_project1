#ifndef __GRID__
#define __GRID__

#include "curve.hpp"

class Grid {
  double delta;
  double t;

public:
  int snapTo1DGrid(Curve &);
  int snapTo2DGrid(Curve &);
  Grid(double);
};

#endif