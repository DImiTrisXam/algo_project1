#include "metrics.hpp"
#include <cmath>
#include <cstdlib>

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