#ifndef __CURVE__
#define __CURVE__

#include "hash.hpp"

class Curve : public Data {
public:
  std::vector<float> tVec;    // time vector
  std::vector<float> gxVec; // grid vector (x-axis)
  std::vector<float> gyVec; // grid vector (y-axis)
  std::vector<float> key;   // LSH key

  Curve(std::vector<float>, std::vector<float>, std::string);
  ~Curve();
  void filter(float);
  void padding();
  void collapseGridToVector();
  void getMinimaMaxima();
  virtual void PRINT() const; /* DEBUG ONLY */
  void printGridCurve();      /* DEBUG ONLY */
  void printKey();            /* DEBUG ONLY */
};

#endif