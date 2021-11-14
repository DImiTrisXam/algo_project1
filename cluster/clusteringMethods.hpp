#ifndef __CLUSTER__
#define __CLUSTER__

#include "../utilities/hash.hpp"
#include <chrono>
#include <vector>

struct Centroid {
  std::vector<float> vec;
  std::vector<float> vecSum; // sum of all points in cluster of centroid
  std::vector<int> indexes;  // indexes of all the points in cluster of centroid
  double silhouette;         // silhouette of centroid's cluster
};

class Cluster {
  std::vector<Data *> points;
  std::vector<Centroid *> centroids;
  int K;              // number of clusters
  std::string method; // method of assignment (Lloyd's algorithm, LSH, Hypercube)
  double overallSilhouette;

  void printCentroids();
  int readInputFile(std::string &);
  int printOutputFile(std::string &, bool, std::chrono::nanoseconds);
  int simpleInitialization();
  int kppInitialization();
  bool LloydsAssignment();
  bool LSHAssignment(std::string &, int, int );
  bool HypercubeAssignment();
  int updateCentroid();
  int Silhouette()

public:
  int begin(std::string &, std::string &, bool, int , int );
  Cluster(int, std::string, std::string &);
  ~Cluster();
};

#endif