#include "../utilities/completeBinaryTree.hpp"
#include "../utilities/grid.hpp"
#include "../utilities/curve.hpp"
#include "../utilities/metrics.hpp"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <typeinfo>
#include <utility>
#include <CUnit/CUnit.h>

int main(int argc, char const *argv[]) {
  std::vector<float> vec1, vec2, vec3, vec4, vec5;
  std::vector<int> tVec;
  int size = 5;

  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  // std::default_random_engine generator(seed);
  std::default_random_engine generator(730);

  std::uniform_real_distribution<float> distribution(0, 100);

  CompleteBinaryTree *tree;
  std::vector<Data *> curves;

  for (auto i = 0; i < size; i++) {
    vec1.push_back(distribution(generator));
    vec2.push_back(distribution(generator));
    vec3.push_back(distribution(generator));
    vec4.push_back(distribution(generator));
    vec5.push_back(distribution(generator));
    tVec.push_back(i + 1);
  }

  auto c1 = (Data *) new Curve(vec1, tVec, "id1");
  auto c2 = (Data *) new Curve(vec2, tVec, "id2");
  auto c3 = (Data *) new Curve(vec3, tVec, "id3");
  auto c4 = (Data *) new Curve(vec4, tVec, "id4");
  auto c5 = (Data *) new Curve(vec4, tVec, "id5");

  curves.push_back(c1);
  curves.push_back(c2);
  curves.push_back(c3);
  curves.push_back(c4);
  curves.push_back(c5);

  tree = new CompleteBinaryTree(curves);

//   auto mean = tree->computeMeanCurve();

  CU_ASSERT(tree->numOfNodes() == 2*curves.size()-1);

  delete tree;

  return 0;
}
