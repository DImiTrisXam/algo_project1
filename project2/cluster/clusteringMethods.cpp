#include "clusteringMethods.hpp"
#include "../search/cubeSearch.hpp"
#include "../search/lshSearch.hpp"
#include "../utilities/metrics.hpp"
#include "../utilities/utilities.hpp"
#include "../utilities/completeBinaryTree.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>

Centroid::Centroid(std::vector<float> &vec, std::string id) : Data(vec, id) {
}

Centroid::~Centroid() {
  // std::cout << "Centroid deleted\n";
}

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

    idToIndexMap[d->id] = points.size() - 1;
  }

  std::cout << "DONE\n";
}

int Cluster::printOutputFile(std::string &name, bool complete, std::chrono::nanoseconds t) {
  std::ofstream file(name);

  std::cout << "Printing to output file... ";

  if (assignMethod.compare("Classic") == 0) {
    file << "Algorithm: AClassic";
  } else if (assignMethod.compare("LSH") == 0) {
    file << "Algorithm: ALSH";
  } else if (assignMethod.compare("Hypercube") == 0) {
    file << "Algorithm: AHypercube";
  } else if (assignMethod.compare("LSH_Frechet") == 0) {
    file << "Algorithm: ALSH_Frechet";
  } else {
    return -1;
  }

  
  if (updateMethod.compare("Mean Frechet") == 0) {
    file << "UMean Frechet\n";
  } else if (updateMethod.compare("Mean Vector") == 0) {
    file << "UMean Vector\n";
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

  // multiply nanoseconds with 10^-9 to print seconds
  file << "clustering_time: " << t.count() * 1e-9 << '\n';

  file << "Silhouette: [" << centroids[0]->silhouette;

  for (auto i = 1; i < centroids.size(); i++) {
    file << ", " << centroids[i]->silhouette;
  }
  file << ", " << overallSilhouette << "]\n";

  std::cout << "DONE\n";
}

// random initialization of K clusters
int Cluster::simpleInitialization() {
  srand(time(0)); // need to set the random seed

  for (auto i = 0; i < K; ++i) {
    auto index = rand() % points.size();
    Centroid *c = new Centroid(points[index]->vec, "centroid");

    centroids.push_back(c);
  }

  return 0;
}

// kmeans++ initialization
int Cluster::kppInitialization(const std::function<double(const Data &, const Data &)> &metric) {
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, points.size() - 1);
  // choose first centroid randomly
  Centroid *c = new Centroid(points[distribution(generator)]->vec, "centroid");
  // c->vec = points[distribution(generator)]->vec;

  centroids.push_back(c);

  for (auto t = 1; t < K; t++) {
    std::vector<double> D; // min distances of each point to some centroid

    for (auto i = 0; i < points.size(); i++) { // for every non-centroid point
      std::vector<double> centroidDist;        // distance of point to every centroid

      for (auto j = 0; j < t; j++) { // for every centroid so far
        auto centroid = (Data *)centroids[j];
        auto dist = metric(*points[i], *centroid);

        if (dist != 0) { // if point is not centroid
          centroidDist.push_back(dist);
        }
      }

      if (!centroidDist.empty()) // if not empty (just for the first loop where t=1)
        D.push_back(*std::min_element(centroidDist.begin(), centroidDist.end()));
    }

    auto Psize = D.size() + 1;
    std::vector<double> P;                             // partial sums of D(i)^2
    auto maxD = *std::max_element(D.begin(), D.end()); // for normalization of D(i)

    P.push_back(0);

    // compute partial sums
    for (auto r = 1; r < Psize; r++) {
      double sum = 0;

      for (auto i = 0; i < r; i++) {
        auto normalizedD_i = D[i] / maxD;
        sum += normalizedD_i * normalizedD_i;
      }

      P.push_back(sum);
    }

    std::uniform_real_distribution<float> distribution(0, P[Psize - 1]);
    auto x = distribution(generator); // pick random float

    // find r with binary search where: x > P[r - 1] && x <= P[r]
    size_t r = std::lower_bound(P.begin(), P.end(), x) - P.begin();
    c = new Centroid(points[r]->vec, "centroid");

    // c->vec = points[r]->vec;
    centroids.push_back(c);
  }

  return 0;
}

// Lloyd's assignment algorithm
bool Cluster::LloydsAssignment(const std::function<double(const Data &, const Data &)> &metric) {
  bool ret = true;
  auto dim = points[0]->vec.size(); // dimension of data

  for (auto &c : centroids) {               // for every centroid
    c->vecSum = std::vector<float>(dim, 0); // initialize to all zeroes
    c->indexes.clear();                     // remove previous iteration's indexes
  }

  for (auto i = 0; i < points.size(); i++) { // for every point
    std::vector<double> centroidDists;

    for (Data *c : centroids) {                        // for every centroid
      centroidDists.push_back(metric(*c, *points[i])); // get distance of point to each centroid
    }

    // find index of smallest distance
    size_t index = std::min_element(centroidDists.begin(), centroidDists.end()) - centroidDists.begin();

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

static HashTable **indexPointsLSH(std::string &inputFile, int L, int k, int w, int dim, int tableSize) {
  std::cout << "indexPointsLSH called" << std::endl;

  auto **tables = new HashTable *[L];

  for (size_t i = 0; i < L; i++) {
    tables[i] = new HashTable(k, 2, dim, tableSize);
  }

  std::string algorithm = "LSH";
  std::string metric;

  readInputFile(inputFile, tables, L, algorithm, metric, 0, nullptr); // put the input in the hash tables

  return tables;
}

static HashTable **indexPointsHypercube(std::string &inputFile, int k, int w, int dim, int tableSize) {
  std::cout << "indexPointsHypercube called" << std::endl;

  HashTable **cube = (HashTable **)new Hypercube *[1];
  cube[0] = (HashTable *)new Hypercube(k, w, dim, tableSize);
  std::string algorithm = "Hypercube";
  std::string metric;

  readInputFile(inputFile, cube, 1, algorithm, metric, 0, nullptr); // put the input in the hypercube

  return cube;
}

bool Cluster::reverseAssignment(int iter, std::string &inputFile, int L, int k, int M, int d, int probes, const std::function<double(const Data &, const Data &)> &metric) {
  bool ret = false;
  int dim = points[0]->vec.size(); // dimension of data
  int w = 2;                       // window for hash table

  static HashTable **indexedPoints;

  if (iter == 0) {
    if (assignMethod.compare("LSH") == 0) {
      int tableSize = points.size() / 8;
      indexedPoints = indexPointsLSH(inputFile, L, k, w, dim, tableSize);
    } else if (assignMethod.compare("Hypercube") == 0) {
      int tableSize = pow(2, d);
      indexedPoints = indexPointsHypercube(inputFile, d, w, dim, tableSize);
    }

    for (auto &c : centroids) {               // for every centroid
      c->vecSum = std::vector<float>(dim, 0); // initialize to all zeroes
    }
  }

  auto minDist = std::numeric_limits<double>::infinity();
  for (auto i = 0; i < centroids.size(); i++) {
    for (auto j = i + 1; j < centroids.size(); j++) {
      auto centroid1 = (Data *)centroids[i];
      auto centroid2 = (Data *)centroids[i];
      double distance = metric(*centroid1, *centroid2);

      if (distance < minDist)
        minDist = distance;
    }
  }

  double radius = pow(2, iter) * (minDist / 2.0);
  static double stopCondition = (10 * points.size()) / 100; // 10% of points are not assigned -> stop
  static int unassignedCounter = points.size();

  for (int i = 0; i < centroids.size(); i++) {
    std::vector<std::string> centroidPoints;
    Data *query = new Data(centroids[i]->vec, "centroid");

    if (assignMethod.compare("LSH") == 0) {
      centroidPoints = approximateRangeSearch(*query, radius, indexedPoints, L, metric); // Reverse assignment through range search with LSH
      // std::cout << "LSH result size " << centroidPoints.size() << ", radius: " << radius << std::endl;
    } else if (assignMethod.compare("Hypercube") == 0) {
      centroidPoints = approximateRangeSearch(*query, radius, *indexedPoints, M, probes, d, metric); // Reverse assignment through range search with hypercube projection
      // std::cout << "Hypercube result size: " << centroidPoints.size() << ", radius: " << radius << std::endl;
    }

    // resolve points' ids to indexes in "points" vector
    for (auto const &point : centroidPoints) {
      auto iter = idToIndexMap.find(point);

      if (iter != idToIndexMap.end()) { // found id in "points" vector
        size_t index = iter->second;
        if (points[index]->cluster == -1) { // point hasn't been assigned to a centroid yet

          points[index]->cluster = i;             // update cluster of point
          centroids[i]->indexes.push_back(index); // add point to cluster

          for (auto j = 0; j < dim; j++) // add point to the sum
            centroids[i]->vecSum[j] += points[index]->vec[j];

          unassignedCounter--;
        } else { // point already assigned to a centroid, compare distances and choose min
          auto centroid = (Data *)centroids[points[index]->cluster];
          double oldDistance = metric(*centroid, *points[index]);
          centroid = (Data *)centroids[i];
          double newDistance = metric(*centroid, *points[index]);

          if (newDistance < oldDistance) {
            std::vector<int> vec = centroids[points[index]->cluster]->indexes; //remove index from old centroid
            vec.erase(std::remove(vec.begin(), vec.end(), index), vec.end());

            for (auto j = 0; j < dim; j++) // remove point from the sum of old cluster
              centroids[i]->vecSum[j] -= points[index]->vec[j];

            points[index]->cluster = i;             // update cluster of point
            centroids[i]->indexes.push_back(index); // add point to cluster
          }
        }
      } else // point is a centroid, assignment to another centroid is invalid
        continue;
    }
  }

  // std::cout << "unassignedCounter: " << unassignedCounter << std::endl;

  if (unassignedCounter <= stopCondition || radius > 50000) {
    ret = true; // stop iterations

    if (unassignedCounter == 0)
      return ret;

    // for every unassigned point, compare its distances to all centroids
    for (int i = 0; i < points.size(); i++) {
      if (points[i]->cluster == -1) { // if not assigned
        minDist = std::numeric_limits<double>::infinity();
        int centroidIndex;

        for (int j = 0; j < centroids.size(); j++) {
          auto centroid = (Data *)centroids[j];
          double distance = metric(*centroid, *points[i]);

          if (distance < minDist) {
            minDist = distance;
            centroidIndex = j;
          }
        }

        points[i]->cluster = centroidIndex;             // update cluster of point
        centroids[centroidIndex]->indexes.push_back(i); // add point to cluster

        for (auto j = 0; j < dim; j++) // add point to the sum
          centroids[centroidIndex]->vecSum[j] += points[i]->vec[j];
      }
    }
  }

  return ret;
}

int Cluster::updateCentroid() {
  auto dim = points[0]->vec.size(); // dimension of data

  // Compute the new centroids
  for (auto i = 0; i < centroids.size(); ++i) {
    if (centroids[i]->indexes.size() != 0) { // if points have been assigned to cluster
      for (auto j = 0; j < dim; j++) {
        centroids[i]->vec[j] = centroids[i]->vecSum[j] / (float)centroids[i]->indexes.size();
      }
    }
  }

  return 0;
}

int Cluster::Silhouette(const std::function<double(const Data &, const Data &)> &metric) {
  auto averageDistSame = new double[points.size()];     // average distance of point to points of same cluster
  auto averageDistNeighbor = new double[points.size()]; // average distance of point to points of best neighbor cluster
  auto silhouette = new double[points.size()];          // silhouettes of points for clustering [-1,1]
  auto nearestNeighbor = new int[points.size()];

  // compute second best clusters for each point
  for (auto i = 0; i < points.size(); i++) { // for every point
    std::vector<double> centroidDists;

    for (Data *c2 : centroids) {           // for every other cluster
      auto dist = metric(*c2, *points[i]); // get distance of point to each centroid
      centroidDists.push_back(dist);
    }

    // change smallest distance to infinity
    centroidDists[points[i]->cluster] = std::numeric_limits<double>::infinity();
    // find index of 2nd closest cluster
    int N = std::min_element(centroidDists.begin(), centroidDists.end()) - centroidDists.begin();

    nearestNeighbor[i] = N;
  }

  for (auto c = 0; c < centroids.size(); c++) { // for every cluster
    double sum = 0;                             // sum of silhouettes of all points in cluster

    for (auto &i : centroids[c]->indexes) { // for every point in cluster
      auto N = nearestNeighbor[i];

      double sum1 = 0; // sum of distances of point to other points in cluster
      double sum2 = 0; // sum of distances of point to other points in next closest cluster

      for (auto &j : centroids[c]->indexes) { // for every other point in cluster
        if (i != j) {                         // if not same point
          sum1 += metric(*points[i], *points[j]);
        }
      }

      for (auto &j : centroids[N]->indexes) { // for every other point in closest neigbor cluster
        sum2 += metric(*points[i], *points[j]);
      }

      averageDistSame[i] = sum1 / (double)centroids[c]->indexes.size();
      averageDistNeighbor[i] = sum2 / (double)centroids[N]->indexes.size();

      auto max = averageDistSame[i] > averageDistNeighbor[i] ? averageDistSame[i] : averageDistNeighbor[i];
      silhouette[i] = (averageDistNeighbor[i] - averageDistSame[i]) / max;

      sum += silhouette[i];
      overallSilhouette += silhouette[i];
    }

    centroids[c]->silhouette = sum / (double)centroids[c]->indexes.size();
  }

  overallSilhouette /= (double)points.size();

  delete[] averageDistSame;
  delete[] averageDistNeighbor;
  delete[] silhouette;
  delete[] nearestNeighbor;

  return 0;
}

int Cluster::begin(std::string &outputFile, std::string &inputFile, bool complete, int L, int k, int M, int d, int probes, const std::function<double(const Data &, const Data &)> &metric) {
  auto maxIterations = points.size();

  auto start = std::chrono::high_resolution_clock::now();

  // simpleInitialization();
  kppInitialization(metric);

  size_t i = 0;
  int flag; // a flag to check if there are no changes in clusters

  // std::cout << "maxIterations: " << maxIterations << "\n";

  for (i = 0; i < maxIterations; i++) {
    if (assignMethod.compare("Classic") == 0) {
      flag = LloydsAssignment(metric);
    } else if (assignMethod.compare("LSH") == 0 || assignMethod.compare("Hypercube") == 0) {
      flag = reverseAssignment(i, inputFile, L, k, M, d, probes, metric);
    } else if (assignMethod.compare("LSH_Frechet") == 0) {
      flag = reverseAssignment(i, inputFile, L, k, M, d, probes, metric);
    } else {
      return -1;
    }

    updateCentroid();

    if (flag)
      break;
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "Iterations: " << i << "\n";
  std::cout << "Computing Silhouette... ";

  // Silhouette(metric);

  std::cout << "DONE\n";

  printOutputFile(outputFile, complete, time);

  return 0;
}

Cluster::Cluster(int K_,std::string amet,std::string umet, std::string &inputFile) : K(K_), assignMethod(amet), updateMethod(umet) {
  readInputFile(inputFile);
}

Cluster::~Cluster() {
    for (auto &p : points) {
        delete p;
    }
    
    for (auto &c : centroids) {
        delete c;
    }
}