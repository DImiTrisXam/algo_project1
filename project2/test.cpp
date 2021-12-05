#include "./utilities/metrics.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

int main(int argc, char const *argv[]) {
  std::vector<float> vec1, vec2;
  std::vector<int> tVec;
  int size = 730;

  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  // std::default_random_engine generator(seed);
  std::default_random_engine generator(730);

  std::uniform_real_distribution<float> distribution(0, 100);

  for (auto i = 0; i < size; i++) {
    vec1.push_back(distribution(generator));
    vec2.push_back(distribution(generator));
    tVec.push_back(i + 1);
  }

  Curve c1(vec1, tVec, "id1");
  Curve c2(vec2, tVec, "id2");

  auto dist = discreteFrechetDist(c1, c2);

  std::cout << "dist: " << dist << '\n';

  // std::ofstream file("test_query_730");

  // for (auto i = 0; i < 5; i++) {
  //   file << "id_" + std::to_string(i + 1) + "\t";
  //   for (auto j = 0; j < size; j++) {
  //     file << distribution(generator) << "\t";
  //   }
  //   file << "\n";
  // }

  return 0;
}
