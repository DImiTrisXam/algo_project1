#ifndef __CURVE__
#define __CURVE__

#include "hash.hpp"

class Curve : public Data {
public:
  std::vector<int> tVec;     // time vector
  std::vector<double> gxVec; // grid vector (x-axis)
  std::vector<double> gyVec; // grid vector (y-axis)
  std::vector<double> key;   // LSH key

  Curve(std::vector<float>, std::vector<int>, std::string);
  void padding();
  void collapseGridToVector();
  virtual void PRINT() const; /* DEBUG ONLY */
  void printGridCurve();      /* DEBUG ONLY */
  void printKey();            /* DEBUG ONLY */
};

#endif