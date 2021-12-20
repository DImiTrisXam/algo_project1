#include "curve.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

Curve_::Curve_(std::vector<float> vec, std::vector<float> tvec, std::string id) : Data(vec, id), tVec(tvec) {
}

Curve_::~Curve_() {
  // std::cout << "Curve_ deleted\n";
}

void Curve_::filter(float epsilon) {

  for (auto i = 1; i < vec.size() - 1; i += 1) {
    if (abs(vec[i] - vec[i - 1]) <= epsilon && abs(vec[i] - vec[i + 1]) <= epsilon) {
      vec.erase(vec.begin() + i);
    }
  }
}

void Curve_::padding() {
  const auto M = std::numeric_limits<float>::max();

  // std::cout << "Padding loop size: " << vec.size() << ", " << gxVec.size() << "\n";

  for (auto i = 0; i < 2 * (vec.size() - gxVec.size()); i++) {
    key.push_back(M);
  }
}

void Curve_::collapseGridToVector() {
  for (auto i = 0; i < gxVec.size(); i++) {
    key.push_back(gxVec[i]);
    key.push_back(gyVec[i]);
  }
}

void Curve_::getMinimaMaxima() {
  std::vector<float> temp;

  temp.push_back(vec[0]);

  for (int i = 1; i < gyVec.size() - 1; i++) {
    if (vec[i] >= std::min(vec[i - 1], vec[i + 1]) && vec[i] <= std::max(vec[i - 1], vec[i + 1]))
      temp.push_back(vec[i]);
  }

  temp.push_back(vec[vec.size() - 1]);

  key = temp;
}

void Curve_::PRINT() const {
  std::cerr << "Id: '" << id << "', ";
  for (auto i = 0; i < vec.size(); i++)
    std::cerr << "(" << tVec[i] << ", " << vec[i] << ") ";
  std::cerr << "\n";
}

void Curve_::printGridCurve() {
  std::cerr << "Id: '" << id << "', ";
  for (auto i = 0; i < vec.size(); i++)
    std::cerr << "(" << gxVec[i] << ", " << gyVec[i] << ") ";
  std::cerr << "\n";
}

void Curve_::printKey() {
  std::cerr << "Id: '" << id << "', ";
  for (auto i = 0; i < key.size(); i++)
    std::cerr << key[i] << " ";
  std::cerr << "\n";
}