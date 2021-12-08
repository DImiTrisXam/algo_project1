#include "metrics.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

double euclidianDist(const std::vector<float> &x, const std::vector<float> &y) {
  double sum = 0, diff;

  if (x.size() != y.size())
    return -1;

  for (int i = 0; i < x.size(); i++) {
    diff = x[i] - y[i];
    sum += diff * diff;
  }

  return sqrt(sum);
}

int manhattanDist(const std::vector<float> &x, const std::vector<float> &y) {
  int sum = 0;

  if (x.size() != y.size())
    return -1;

  for (int i = 0; i < x.size(); i++)
    sum += abs(x[i] - y[i]);

  return sum;
}

int hammingDist(int n1, int n2) {
  int x = n1 ^ n2; // XOR the bits
  int dist = 0;

  while (x > 0) {  // go through all bits
    dist += x & 1; // add least significant bit to distance
    x >>= 1;       // shift 1 bit to the right (go to next bit)
  }

  return dist;
}

double discreteFrechetDist(const Curve &x, const Curve &y) {
  auto m1 = x.vec.size();
  auto m2 = y.vec.size();

  auto distArray = new double[m1 * m2]; // array for dynamic programming

  for (auto i = 0; i < m1; ++i) { // compute distance matrix
    for (auto j = 0; j < m2; ++j) {
      // std::cout << "i: " << i << ",j: " << j << '\n';

      std::vector<float> x_i{(float)x.tVec[i], x.vec[i]}; // i-th element of Curve x
      std::vector<float> y_j{(float)y.tVec[j], y.vec[j]}; // j-th element of Curve y

      if (i == 0 && j == 0) // if first cell
        distArray[0] = euclidianDist(x_i, y_j);
      else if (i == 0 && j > 0) { // if first row
        // distArray[j] = distArray[0][j]
        distArray[j] = std::max(distArray[j - 1], euclidianDist(x_i, y_j));
      } else if (i > 0 && j == 0) { // if first column
        // distArray[i* m2] = distArray[i][0]
        distArray[i * m2] = std::max(distArray[(i - 1) * m2], euclidianDist(x_i, y_j));
      } else if (i > 0 && j > 0) { // anything else
        // distArray[(i - 1) * m2 + j] = distArray[i-1][j]
        auto d1 = distArray[(i - 1) * m2 + j];
        // distArray[(i - 1) * m2 + j - 1] = distArray[i-1][j-1]
        auto d2 = distArray[(i - 1) * m2 + j - 1];
        // distArray[i * m2 + j - 1] = distArray[i][j-1]
        auto d3 = distArray[i * m2 + j - 1];

        double min = std::min(d1, std::min(d2, d3));

        // distArray[i * m2 + j] = distArray[i][j]
        distArray[i * m2 + j] = std::max(min, euclidianDist(x_i, y_j));
      }
    }
  }

  auto res = distArray[m1 * m2 - 1];

  delete[] distArray;

  return res;
}