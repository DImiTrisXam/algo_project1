#include "curve.hpp"
#include <iostream>
#include <limits>

Curve::Curve(std::vector<float> vec, std::vector<int> tvec, std::string id) : Data(vec, id), tVec(tvec) {
}

void Curve::padding() {
  const auto M = std::numeric_limits<double>::max();

  for (auto i = gxVec.size(); i < vec.size(); i++) {
    gxVec.push_back(M);
    gyVec.push_back(M);
  }
}

void Curve::collapseGridToVector() {
  for (auto i = 0; i < gxVec.size(); i++) {
    key.push_back(gxVec[i]);
    key.push_back(gyVec[i]);
  }
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

void Curve::printKey() {
  std::cerr << "Id: '" << id << "', ";
  for (auto i = 0; i < key.size(); i++)
    std::cerr << key[i] << " ";
  std::cerr << "\n";
}