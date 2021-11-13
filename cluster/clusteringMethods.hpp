#ifndef __CLUSTER__
#define __CLUSTER__

#include "../utilities/hash.hpp"
#include <vector>
#include <chrono>

struct Centroid {
  std::vector<float> vec;
  std::vector<float> vecSum; // sum of all points in cluster of centroid
  std::vector<int> indexes;  // indexes of all the points in cluster of centroid
};

class Cluster {
  std::vector<Data *> points;
  std::vector<Centroid *> centroids;
  int K;              // number of clusters
  std::string method; // method of assignment (Lloyd's algorithm, LSH, Hypercube)

  void printCentroids();
  int readInputFile(std::string &);
  int printOutputFile(std::string &, bool, std::chrono::nanoseconds);
  int simpleInitialization();
  int kppInitialization();
  bool LloydsAssignment();
  bool LSHAssignment();
  bool HypercubeAssignment();
  int updateCentroid();

public:
  int begin(std::string &, bool);
  Cluster(int, std::string, std::string &);
  ~Cluster();
};

#endif