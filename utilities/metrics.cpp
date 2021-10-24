#include "metrics.hpp"
#include <cmath>
#include <cstdlib>

int euclidianDist(std::vector<int> x, std::vector<int> y) {
  double sum = 0, diff;

  if (x.size() != y.size())
    return -1;

  for (int i = 0; i < x.size(); i++) {
    diff = x[i] - y[i];
    sum += diff * diff;
  }

  return (int)sqrt(sum);
}

int manhattanDist(std::vector<int> x, std::vector<int> y) {
  int sum = 0;

  if (x.size() != y.size())
    return -1;

  for (int i = 0; i < x.size(); i++)
    sum += abs(x[i] - y[i]);

  return sum;
}