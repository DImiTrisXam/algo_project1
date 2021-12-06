#include "curve.hpp"
#include <iostream>

Curve::Curve(std::vector<float> vec, std::vector<int> tvec, std::string id) : Data(vec, id), tVec(tvec) {
}

void Curve::PRINT() const {
  std::cerr << "Id: '" << id << "', ";
  for (auto i = 0; i < vec.size(); i++)
    std::cerr << "(" << tVec[i] << ", " << vec[i] << ") ";
  std::cerr << "\n";
}

void Curve::printGridCurve() {
  std::cerr << "Id: '" << id << "', ";
  for (auto i = 0; i < vec.size(); i++)
    std::cerr << "(" << gxVec[i] << ", " << gyVec[i] << ") ";
  std::cerr << "\n";
}