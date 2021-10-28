#include "hash.hpp"
#include <chrono>
#include <climits>
#include <cmath>
#include <iostream>
#include <random>

float dotProduct(const std::vector<float>& x, const std::vector<float>& y) {
  float product = 0;

  for (size_t i = 0; i < x.size(); i++)
    product += x[i] * y[i];

  return product;
}

int hFunc(const std::vector<float>& p, int w) {
  // Create random engine with the help of seed
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  // std::random_device rd;
  // std::mt19937 generator(rd());

  std::normal_distribution<float> distN(0.0, 1.0);            // standard normal distribution
  std::uniform_real_distribution<float> distU(0.0, (float)w); // uniform real distribution
  std::vector<float> v;
  float t;

  // std::cout << "\nNormal distribution for " << p.size() << " samples (mean=0, standard deviation=1) =>\n";

  // pick v's coordinates from normal distribution
  for (size_t i = 1; i <= p.size(); i++) {
    // std::cout << i << ". " << distN(generator) << "\n";
    v.push_back(distN(generator));
  }

  t = distU(generator); // pick t from uniform distribution

  return (int)floor((dotProduct(p, v) + t) / (float)w);
}

int ID(const std::vector<float> &p, int k, int w) {
  int sum = 0, r;
  unsigned int M = UINT_MAX - 4; // 2^32 - 5

  // Create random engine with the help of seed
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_int_distribution<int> distU(-10, 10); // uniform int distribution

  for (size_t i = 0; i < k; i++) {
    r = distU(generator); // pick r from uniform distribution
    sum += r * hFunc(p, w);
  }

  return sum % M;
}

int gFunc(const std::vector<float> &p, int k, int w) {
  int tableSize = 1000;

  return ID(p, k, w) % tableSize;
}