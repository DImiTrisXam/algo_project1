#include "metrics.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

double euclidianDist(const Data &a, const Data &b) {
  double sum = 0, diff;
  auto x = a.vec;
  auto y = b.vec;

  if (x.size() != y.size())
    return -1;

  for (int i = 0; i < x.size(); i++) {
    diff = x[i] - y[i];
    sum += diff * diff;
  }

  return sqrt(sum);
}

int manhattanDist(const Data &a, const Data &b) {
  int sum = 0;
  auto x = a.vec;
  auto y = b.vec;

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

double discreteFrechetDist(const Data &a, const Data &b) {
  auto x = (const Curve &)a;
  auto y = (const Curve &)b;
  auto m1 = x.vec.size();
  auto m2 = y.vec.size();
  double *distArray;

  try {
    distArray = new double[m1 * m2]; // array for dynamic programming
  } catch (const std::bad_alloc &e) {
    std::cout << "Allocation failed in discreteFrechetDist: " << e.what() << '\n';
  }

  // if (!distArray) // out of heap
  //   throw "Unable to allocate array in discreteFrechetDist. Out of heap memory.";

  for (auto i = 0; i < m1; ++i) { // compute distance matrix
    for (auto j = 0; j < m2; ++j) {
      // std::cout << "i: " << i << ",j: " << j << '\n';

      std::vector<float> temp1{(float)x.tVec[i], x.vec[i]}; // i-th element of Curve x
      std::vector<float> temp2{(float)y.tVec[j], y.vec[j]}; // j-th element of Curve y

      Data x_i(temp1, "temp1");
      Data y_j(temp2, "temp2");

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