#ifndef __CLUSTER__
#define __CLUSTER__

#include "../utilities/hash.hpp"
#include <vector>

class Cluster {
  std::vector<Data *> points;
  std::vector<std::vector<float>> centroids;
  int K;              // number of clusters
  std::string method; // method of assignment (Lloyd's algorithm, LSH, Hypercube)

  int readInputFile(std::string &);
  int simpleInitialization();
  int kppInitialization();
  int LloydsAssignment();
  int LSHAssignment();
  int HypercubeAssignment();
  int updateCentroid();

public:
  int begin(std::string &);
  Cluster(int, std::string);
  ~Cluster();
};

#endif