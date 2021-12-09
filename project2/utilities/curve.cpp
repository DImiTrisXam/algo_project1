#include "curve.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <cmath>

Curve::Curve(std::vector<float> vec, std::vector<int> tvec, std::string id) : Data(vec, id), tVec(tvec) {
}

void Curve::filter(float epsilon) {
  
  for (auto i = 1; i < vec.size()-1; i+=1) {
      if (abs(vec[i] - vec[i-1]) <= epsilon && abs(vec[i] - vec[i+1]) <= epsilon) {
          vec.erase(vec.begin() + i);
      }
  }
  
}

void Curve::padding() {
  const auto M = std::numeric_limits<float>::max();

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

void Curve::getMinimaMaxima() {
  std::vector<float> temp;

  temp.push_back(vec[0]);

  for (int i = 1; i < gyVec.size() - 1; i++) {
    if (vec[i] >= std::min(vec[i - 1], vec[i + 1]) && vec[i] <= std::max(vec[i - 1], vec[i + 1]))
      temp.push_back(vec[i]);
  }

  temp.push_back(vec[vec.size() - 1]);

  key = temp;
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