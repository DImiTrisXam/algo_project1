#ifndef __CLUSTER__
#define __CLUSTER__

#include "../utilities/hash.hpp"
#include <chrono>
#include <unordered_map>
#include <vector>

struct Centroid {
  std::vector<float> vec;
  std::vector<float> vecSum; // sum of all points in cluster of centroid
  std::vector<int> indexes;  // indexes of all the points in cluster of centroid
  double silhouette;         // silhouette of centroid's cluster
};

class Cluster {
  std::vector<Data *> points;
  std::unordered_map<std::string, size_t> idToIndexMap;
  std::vector<Centroid *> centroids;
  int K;              // number of clusters
  std::string method; // method of assignment (Lloyd's algorithm, LSH, Hypercube)
  double overallSilhouette;

  void printCentroids();
  int readInputFile(std::string &);
  int printOutputFile(std::string &, bool, std::chrono::nanoseconds);
  int simpleInitialization();
  int kppInitialization(const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
  bool LloydsAssignment(const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
  bool reverseAssignment(int, std::string &, int, int, int, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
  int updateCentroid();
  int Silhouette(const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);

public:
  int begin(std::string &, std::string &, bool, int, int, int, int, const std::function<double(const std::vector<float> &, const std::vector<float> &)> &);
  Cluster(int, std::string, std::string &);
  ~Cluster();
};

#endif