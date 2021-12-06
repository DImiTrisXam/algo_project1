#include "grid.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

Grid::Grid(double delta_) : delta(delta_) {
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_real_distribution<double> distribution(0.0, delta);

  t = distribution(generator); // pick random t
}

int Grid::snapTo1DGrid(Curve &c) {
  // snap curve to 1D grid
  for (auto i = 0; i < c.vec.size(); i++) {
    c.gyVec.push_back(round((c.vec[i] - t) / delta) * delta - t);
  }

  return 0;
}

int Grid::snapTo2DGrid(Curve &c) {
  // snap curve to 2D grid
  for (auto i = 0; i < c.vec.size(); i++) {
    c.gxVec.push_back(round((c.tVec[i] - t) / delta) * delta - t);
    c.gyVec.push_back(round((c.vec[i] - t) / delta) * delta - t);
  }

  // remove consecutive duplicates
  std::vector<double> temp1;
  std::vector<double> temp2;

  for (int i = 0; i < c.gxVec.size(); i++) {
    if (c.gxVec[i] != c.gxVec[i + 1] && c.gyVec[i] != c.gyVec[i + 1]) { // if not consecutive duplicate
      temp1.push_back(c.gxVec[i]);
      temp2.push_back(c.gyVec[i]);
    }
  }

  c.gxVec = temp1;
  c.gyVec = temp2;

  return 0;
}