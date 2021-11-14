#include "clusteringMethods.hpp"
#include "../utilities/metrics.hpp"
#include "../utilities/utilities.hpp"
#include "../lsh/lshSearch.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>

void Cluster::printCentroids() {
  for (auto i = 0; i < centroids.size(); i++) {
    std::cout << "CLUSTER-" << i + 1 << " { size: " << centroids[i]->indexes.size() << ", [";

    for (auto j = 0; j < centroids[i]->vec.size(); j++) {
      if (j == centroids[i]->vec.size() - 1) // if last coordinate
        std::cout << centroids[i]->vec[j];
      else
        std::cout << centroids[i]->vec[j] << ", ";
    }

    std::cout << "]}\n";
  }
}

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

int Cluster::printOutputFile(std::string &name, bool complete) {
  std::ofstream file(name);
  std::string line;

  std::cout << "Printing to output file... ";

  if (method.compare("Classic") == 0) {
    file << "Algorithm: Lloyds\n";
  } else if (method.compare("LSH") == 0) {
    file << "Algorithm: Range Search LSH\n";
  } else if (method.compare("Hypercube") == 0) {
    file << "Algorithm: Range Search Hypercube\n";
  } else {
    return -1;
  }

  for (auto i = 0; i < centroids.size(); i++) {
    file << "CLUSTER-" << i + 1 << " { size: " << centroids[i]->indexes.size() << ", [";

    for (auto j = 0; j < centroids[i]->vec.size(); j++) {
      if (j == centroids[i]->vec.size() - 1) // if last coordinate
        file << centroids[i]->vec[j];
      else
        file << centroids[i]->vec[j] << ", ";
    }

    file << "]}\n";
  }

  if (complete) {
    for (auto i = 0; i < centroids.size(); i++) {
      file << "CLUSTER-" << i + 1 << " { centroid, ";

      for (auto j = 0; j < centroids[i]->indexes.size(); j++) {
        if (j == centroids[i]->indexes.size() - 1) // if last id
          file << points[j]->id;
        else
          file << points[j]->id << ", ";
      }

      file << "}\n";
    }
  }

  std::cout << "DONE\n";
}

// random initialization of K clusters
int Cluster::simpleInitialization() {
  srand(time(0)); // need to set the random seed

  for (auto i = 0; i < K; ++i) {
    auto index = rand() % points.size();
    Centroid *c = new Centroid;

    c->vec = points[index]->vec;

    centroids.push_back(c);
  }

  // printCentroids();

  return 0;
}

// kmeans++ initialization
int Cluster::kppInitialization() {
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, points.size() - 1);

  Centroid *c = new Centroid;
  // choose first centroid randomly
  c->vec = points[distribution(generator)]->vec;

  centroids.push_back(c);

  for (auto t = 1; t < K; t++) {
    std::vector<double> D; // min distances of each point to some centroid

    for (auto i = 0; i < points.size(); i++) { // for every non-centroid point
      std::vector<double> centroidDist;        // distance of point to every centroid

      for (auto j = 0; j < t; j++) { // for every centroid so far
        auto dist = euclidianDist(points[i]->vec, centroids[j]->vec);

        if (dist != 0) { // if point is not centroid
          centroidDist.push_back(dist);
        }
      }

      if (!centroidDist.empty()) // if not empty (just for the first loop where t=1)
        D.push_back(*std::min_element(centroidDist.begin(), centroidDist.end()));
    }

    auto Psize = D.size() + 1;
    auto P = new double[Psize];                        // partial sums of D(i)^2
    auto maxD = *std::max_element(D.begin(), D.end()); // for normalization of D(i)

    P[0] = 0;

    // compute partial sums
    for (auto r = 1; r < Psize; r++) {
      double sum = 0;

      for (auto i = 0; i < r; i++) {
        auto normalizedD_i = D[i] / maxD;
        sum += normalizedD_i * normalizedD_i;
      }

      P[r] = sum;
    }

    std::uniform_real_distribution<float> distribution(0, P[Psize - 1]);
    auto x = distribution(generator); // pick random float

    for (auto r = 1; r < Psize; r++) {
      if (x > P[r - 1] && x <= P[r]) {
        c = new Centroid;

        c->vec = points[r]->vec;
        centroids.push_back(c);

        break;
      }
    }

    delete P;
  }

  // printCentroids();

  return 0;
}

// Lloyd's assignment algorithm
bool Cluster::LloydsAssignment() {
  bool ret = true;
  auto dim = points[0]->vec.size(); // dimension of data

  for (auto &c : centroids) {               // for every centroid
    c->vecSum = std::vector<float>(dim, 0); // initialize to all zeroes
    c->indexes.clear();                     // remove previous iteration's indexes
  }

  for (int i = 0; i < points.size(); i++) { // for every point
    std::vector<double> centroidDists;

    for (auto &c : centroids) {                                       // for every centroid
      centroidDists.push_back(euclidianDist(c->vec, points[i]->vec)); // get distance of point to each centroid
    }

    // find index of smallest distance
    size_t index = std::min_element(centroidDists.begin(), centroidDists.end()) - centroidDists.begin();

    points[i]->minDist = centroidDists[index];

    if (points[i]->cluster != index) { // if there is change
      ret = false;                     // continue iterations
      points[i]->cluster = index;      // update cluster of point
    }

    centroids[index]->indexes.push_back(i); // add point to cluster

    for (auto j = 0; j < dim; j++) // add point to the sum
      centroids[index]->vecSum[j] += points[i]->vec[j];
  }

  return ret;
}

static HashTable** indexPointsLSH(std::string &inputFile, int L, int k){
    std::cout << "indexPointsLSH called" << std::endl;
    int dim = 0; // dimension of data
    int numOfInputs = readNumberOfLines(inputFile, dim);
    int tableSize = numOfInputs / 8;
    int w = 2; // window for hash table
    auto **tables = new HashTable *[L];

    for (size_t i = 0; i < L; i++) {
        tables[i] = new HashTable(k, 2, dim, tableSize);
    }

    readInputFile(inputFile, tables, L); // put the input in the hash tables

    return tables;
}

// Reverse assignment through range search with LSH
bool Cluster::LSHAssignment(std::string &inputFile, int L, int k) {
    static HashTable **tablesLSH = indexPointsLSH(inputFile, L, k); // create LSH tables once for entire algorithm
    bool ret = false;
    auto dim = points[0]->vec.size(); // dimension of data

    for (auto &c : centroids) {               // for every centroid
        c->vecSum = std::vector<float>(dim, 0); // initialize to all zeroes
        c->indexes.clear();                     // remove previous iteration's indexes
    }

    auto minDist = std::numeric_limits<double>::infinity();
    for(auto i = 0; i < centroids.size(); i++){
        for(auto j = i + 1; j < centroids.size(); j++){
            double distance = euclidianDist(centroids[i]->vec, centroids[j]->vec);
            if (distance < minDist)
                minDist = distance;
        }
    }

    double radius = (double)(minDist / 2);
    std::vector<bool> assignedPoints(points.size(), false);   // initialize vector with false values
    double stopCondition = (( 10 * points.size() ) / 100);      // 10% of points are not assigned -> stop
    int unassignedCounter = points.size();

    while(true){
        for (int i = 0; i <= centroids.size(); i++){
            Data *query = new Data(centroids[i]->vec, "centroid");
            std::vector<std::string> centroidPoints = approximateRangeSearch(*query, radius, tablesLSH, L);
            
            // resolve points' ids to indexes in "points" vector
            for (auto const &point : centroidPoints){
                auto iter = std::find_if(points.begin(), points.end(), [=](Data* data) { return data->id == point; });
                if(iter != points.end()){           // found id in "points" vector
                    auto index = iter - points.begin();
                    if (assignedPoints[index] == false){    // point hasn't been assigned to a centroid yet
                        points[index]->cluster = i;       // update cluster of point
                        centroids[i]->indexes.push_back(index); // add point to cluster
                        assignedPoints[index] = true;           // set point assignment to true
                    }
                    else{       // point already assigned to a centroid, compare distances and choose min
                        double oldDistance = euclidianDist(centroids[points[index]->cluster]->vec, points[index]->vec);
                        double newDistance = euclidianDist(centroids[i]->vec, points[index]->vec);
                        if (newDistance < oldDistance){
                            points[index]->cluster = i;             // update cluster of point
                            centroids[i]->indexes.erase(centroids[i]->indexes.begin() + index);          //remove index from old centroid
                            centroids[i]->indexes.push_back(index); //add point to cluster
                        }
                    }
                    unassignedCounter--;
                }
                else            // point is a centroid, assignment to another centroid is invalid
                    continue;
            }
            
        }
        if (unassignedCounter <= stopCondition){
            ret = true;     // stop iterations
            break;
        }
        radius *= 2;
    }

    // for every unassigned point, compare its distances to all centroids
    for (int i = 0; i < points.size(); i++){
        if (assignedPoints[i] == false){
            minDist = std::numeric_limits<double>::infinity();
            int centroidIndex;
            for (int j = 0; j <= centroids.size(); j++){
                double distance = euclidianDist(centroids[j]->vec, points[i]->vec);
                if (distance < minDist){
                    minDist = distance;
                    centroidIndex = j;
                }
            }
            points[i]->cluster = centroidIndex;       // update cluster of point
            centroids[centroidIndex]->indexes.push_back(i); // add point to cluster
        }
    }

    return ret;
}

// Reverse assignment through range search with hypercube projection
bool Cluster::HypercubeAssignment() {
  bool ret = true;
  return ret;
}

int Cluster::updateCentroid() {
  auto dim = points[0]->vec.size(); // dimension of data

  // Compute the new centroids
  for (auto i = 0; i < centroids.size(); ++i) {
    for (auto j = 0; j < dim; j++) {
      centroids[i]->vec[j] = centroids[i]->vecSum[j] / (float)centroids[i]->indexes.size();
    }
  }

  return 0;
}

int Cluster::begin(std::string &outputFile, std::string &inputFile, bool complete, int L, int k) {
  auto maxIterations = points.size();

  // simpleInitialization();
  kppInitialization();

  size_t i = 0;
  int flag; // a flag to check if there are no changes in clusters

  // std::cout << "maxIterations: " << maxIterations << "\n";

  for (i = 0; i < maxIterations; i++) {
    if (method.compare("Classic") == 0) {
      flag = LloydsAssignment();
    } else if (method.compare("LSH") == 0) {
      flag = LSHAssignment(inputFile, L, k);
    } else if (method.compare("Hypercube") == 0) {
      flag = HypercubeAssignment();
    } else {
      return -1;
    }

    if (flag)
      break;

    updateCentroid();
  }

  std::cout << "Iterations: " << i + 1 << "\n";

  printOutputFile(outputFile, complete);

  return 0;
}

Cluster::Cluster(int K_, std::string met, std::string &inputFile) : K(K_), method(met) {
  readInputFile(inputFile);
}

Cluster::~Cluster() {}
