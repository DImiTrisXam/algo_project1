#include "./utilities/grid.hpp"
#include "./utilities/hash.hpp"
#include "./utilities/metrics.hpp"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <typeinfo>
#include <utility>

std::list<std::pair<size_t, size_t>> optimalTraversal(const Data &a, const Data &b) {
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

  std::list<std::pair<size_t, size_t>> traversal;

  auto P_i = m1 - 1;
  auto Q_i = m2 - 1;

  std::pair<size_t, size_t> tuple(P_i, Q_i);

  traversal.push_front(tuple);

  while (P_i > 0 && Q_i > 0) {
    auto d1 = distArray[(P_i - 1) * m2 + Q_i];     // distArray[P_i - 1][Q_i]
    auto d2 = distArray[P_i * m2 + Q_i - 1];       // distArray[P_i][Q_i-1]
    auto d3 = distArray[(P_i - 1) * m2 + Q_i - 1]; // distArray[P_i-1][Q_i-1]
    auto min = std::min(d1, std::min(d2, d3));

    if (min == d1) {
      std::pair<size_t, size_t> tuple(--P_i, Q_i);
      traversal.push_front(tuple);
    } else if (min == d2) {
      std::pair<size_t, size_t> tuple(P_i, --Q_i);
      traversal.push_front(tuple);
    } else {
      std::pair<size_t, size_t> tuple(--P_i, --Q_i);
      traversal.push_front(tuple);
    }
  }

  delete[] distArray;

  return traversal;
}

Curve *meanDiscreteFrechetCurve(const Data &a, const Data &b) {
  auto traversal = optimalTraversal(a, b);

  auto x = (const Curve &)a;
  auto y = (const Curve &)b;
  std::vector<float> vec;
  std::vector<int> tVec;
  int i = 0;

  for (const auto tuple : traversal) {
    vec.push_back((x.vec[tuple.first] + y.vec[tuple.second]) / 2);
    tVec.push_back(++i);
  }

  Curve *mean = new Curve(vec, tVec, "mean");

  return mean;
}

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

  // auto d1 = new Data(vec1, "id1");
  // auto d2 = new Curve(vec1, tVec, "id1");
  // auto d3 = (Data *)new Curve(vec1, tVec, "id1");

  // std::cout << typeid(*d1).name() << "\n";
  // std::cout << typeid(*d2).name() << "\n";
  // std::cout << typeid(*d3).name() << "\n";

  // auto type1 = typeid(*d2).name();
  // auto type2 = typeid(*d3).name();
  // auto type3 = typeid(Curve).name();

  // if (strcmp(type1, type2) == 0) {
  //   std::cout << "WOOOOOOOOO\n";
  // }

  // if (strcmp(type1, type3) == 0) {
  //   std::cout << "AAAAAAAAAA\n";
  // }

  auto start = std::chrono::high_resolution_clock::now();

  auto dist = discreteFrechetDist(c1, c2);

  auto end = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "dist: " << dist << ", time: " << time.count() * 1e-9 << " seconds\n\n";

  start = std::chrono::high_resolution_clock::now();

  auto traversal = optimalTraversal(c1, c2);

  end = std::chrono::high_resolution_clock::now();
  time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "traversal: " << traversal.size() << ", time: " << time.count() * 1e-9 << " seconds\n\n";

  // for (const auto tuple : traversal) {
  //   std::cout << "(" << tuple.first << "," << tuple.second << ")"
  //             << " ";
  // }
  // std::cout << "\n";

  start = std::chrono::high_resolution_clock::now();

  Curve *c = meanDiscreteFrechetCurve(c1, c2);

  end = std::chrono::high_resolution_clock::now();
  time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "Mean curve time: " << time.count() * 1e-9 << " seconds\n\n";

  // c->PRINT();

  delete c;

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
