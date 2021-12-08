#include "./utilities/grid.hpp"
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
  
  std::cout << "dist: " << dist << "\n\n";

  // double delta = 2;

  // Grid grid(delta);

  // snap curves
  // grid.snapTo2DGrid(c1);
  // grid.snapTo2DGrid(c2);

  // std::cout << "c1 size after snapping: " << c1.gxVec.size() << "\n\n";
  // c1.printGridCurve();
  // std::cout << "c2 size after snapping: " << c2.gxVec.size() << "\n\n";
  // c2.printGridCurve();

  // c1.padding();
  // std::cout << "c1 size after padding: " << c1.gxVec.size() << "\n\n";
  // c1.printGridCurve();
  // c2.padding();
  // std::cout << "c2 size after padding: " << c2.gxVec.size() << "\n\n";
  // c2.printGridCurve();

  // LSH vector key for 2D grid
  // c1.collapseGridToVector();
  // c1.printKey();
  // c2.collapseGridToVector();
  // c2.printKey();

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

  return 0;
}
