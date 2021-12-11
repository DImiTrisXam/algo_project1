#include "./utilities/grid.hpp"
#include "./utilities/hash.hpp"
#include "./utilities/metrics.hpp"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <typeinfo>

int main(int argc, char const *argv[]) {
  std::vector<float> vec1, vec2;
  std::vector<int> tVec;
  int size = 120;
  HashTable table(14, 2, 120, 10);

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

  auto d1 = new Data(vec1, "id1");
  auto d2 = new Curve(vec1, tVec, "id1");
  auto d3 = (Data *)new Curve(vec1, tVec, "id1");

  std::cout << typeid(*d1).name() << "\n";
  std::cout << typeid(*d2).name() << "\n";
  std::cout << typeid(*d3).name() << "\n";

  auto type1 = typeid(*d2).name();
  auto type2 = typeid(*d3).name();
  auto type3 = typeid(Curve).name();

  if (strcmp(type1, type2) == 0) {
    std::cout << "WOOOOOOOOO\n";
  }

  if (strcmp(type1, type3) == 0) {
    std::cout << "AAAAAAAAAA\n";
  }

  // auto start = std::chrono::high_resolution_clock::now();

  // auto dist = discreteFrechetDist(c1, c2);

  // auto end = std::chrono::high_resolution_clock::now();
  // auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  // std::cout << "dist: " << dist << ", time: " << time.count() * 1e-9 << " seconds\n\n";

  // float averageVariation = 0;

  // for (auto i = 0; i < size - 1; i++) {
  //   averageVariation += abs(c1.vec[i + 1] - c1.vec[i]);
  // }

  // averageVariation = averageVariation / size;

  // float epsilon = (averageVariation * 160) / 100;

  // std::cout << "averageVariation: " << epsilon << "\n\n";

  // c1.filter(epsilon);

  // std::cout << "c1 size after filtering: " << c1.vec.size() << "\n\n";

  // c1.PRINT();

  // double delta = 2;

  // Grid grid(delta);

  // snap curves
  // grid.snapTo2DGrid(c1);
  // grid.snapTo2DGrid(c2);

  // std::cout << "c1 size after snapping: " << c1.gxVec.size() << "\n\n";
  // c1.printGridCurve();
  // std::cout << "c2 size after snapping: " << c2.gxVec.size() << "\n\n";
  // c2.printGridCurve();

  // LSH vector key for 2D grid
  // c1.collapseGridToVector();
  // c2.collapseGridToVector();
  // c2.printKey();

  // c1.padding();
  // std::cout << "c1 size after padding: " << c1.key.size() << "\n\n";
  // c1.printKey();
  // c1.printGridCurve();
  // c2.padding();
  // std::cout << "c2 size after padding: " << c2.gxVec.size() << "\n\n";
  // c2.printGridCurve();

  // snap curves
  // grid.snapTo1DGrid(c1);
  // grid.snapTo1DGrid(c2);

  // LSH vector key for 1D grid
  // c1.getMinimaMaxima();
  // std::cout << "c1 key size: " << c1.key.size() << "\n\n";
  // c1.printKey();
  // c2.collapseGridToVector();
  // std::cout << "c2 key size: " << c2.key.size() << "\n\n";
  // c2.printKey();

  // add curve to table
  // table.add(c1.vec, c1.tVec, c1.key, c1.id);

  // table.PRINT();

  return 0;
}
