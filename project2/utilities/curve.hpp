#ifndef __CURVE__
#define __CURVE__

#include "hash.hpp"

class Curve : public Data {
public:
  std::vector<int> tVec; // time vector

  Curve(std::vector<float>, std::vector<int>, std::string);
  virtual void PRINT() const; /* DEBUG ONLY */
};

#endif