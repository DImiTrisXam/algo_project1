#include "clusteringMethods.hpp"
#include "../utilities/metrics.hpp"
// #include "../utilities/utilities.hpp"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>

int Cluster::readInputFile(std::string &name) {
  std::ifstream file(name);
  std::string line;

  std::cout << "Processing input file... ";

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string id;
    std::vector<float> vec;
    float temp;

    ss >> id;

    while (ss >> temp)
      vec.push_back(temp);

    Data *d = new Data(vec, id);

    points.push_back(d);
  }

  std::cout << "DONE\n";
}

// random initialization of K clusters
int Cluster::simpleInitialization() {
  srand(time(0)); // need to set the random seed

  for (auto i = 0; i < K; ++i) {
    centroids.push_back(points[rand() % points.size()]->vec);
  }
}

// kmeans++ initialization
int Cluster::kppInitialization() {
}

// Lloyd's assignment algorithm
int Cluster::LloydsAssignment() {
  int ret = 1;

  for (auto i = 0; i < centroids.size(); ++i) {

    for (auto &p : points) {
      double dist = euclidianDist(centroids[i], p->vec);

      if (dist < p->minDist) {
        p->minDist = dist;

        if (p->cluster != i) { // if there is change
          ret = 0;
          p->cluster = i;
        }
      }
    }
  }

  return ret;
}

// Reverse assignment through range search with LSH
int Cluster::LSHAssignment() {}

// Reverse assignment through range search with hypercube projection
int Cluster::HypercubeAssignment() {}

int Cluster::updateCentroid() {
  auto dim = points[0]->vec.size(); // dimension of data
  std::vector<int> nPoints(K, 0);   // # of points for each cluster (initialise with zeroes)
  std::vector<float> sum(dim, 0);   // vector sum of all points (initialise with zeroes)

  for (const auto &p : points) {
    nPoints[p->cluster] += 1;

    for (auto i = 0; i < dim; i++) { // add point to the sum
      sum[i] += p->vec[i];
    }

    p->minDist = std::numeric_limits<double>::infinity(); // reset minDist to infinity
  }

  // Compute the new centroids
  for (auto i = 0; i < centroids.size(); ++i) {
    for (auto j = 0; j < dim; j++) {
      centroids[i][j] = sum[j] / nPoints[i];
    }
  }
}

int Cluster::begin(std::string &inputFile) {
  int maxIterations = points.size();
  // bool flag = false; // a flag to check if there are no changes in clusters

  readInputFile(inputFile);
  simpleInitialization();

  int (Cluster::*AssignmentFunc)(); // function pointer

  if (method.compare("Classic") == 0) {
    AssignmentFunc = &Cluster::LloydsAssignment;
  } else if (method.compare("LSH") == 0) {
    AssignmentFunc = &Cluster::LSHAssignment;
  } else if (method.compare("Hypercube") == 0) {
    AssignmentFunc = &Cluster::HypercubeAssignment;
  } else {
    return -1;
  }

  for (auto i = 0; i < maxIterations; i++) {
    if (AssignmentFunc) // if no changes break loop
      break;
    updateCentroid();
  }

  return 0;
}

Cluster::Cluster(int K_, std::string met) : K(K_), method(met) {}

Cluster::~Cluster() {}
