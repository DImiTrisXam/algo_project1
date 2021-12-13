#ifndef __CLUSTER__
#define __CLUSTER__

#include "../utilities/hash.hpp"
#include <chrono>
#include <unordered_map>
#include <vector>

struct Centroid : public Data {
  std::vector<float> vecSum; // sum of all points in cluster of centroid
  std::vector<int> indexes;  // indexes of all the points in cluster of centroid
  double silhouette;         // silhouette of centroid's cluster

  Centroid(std::vector<float> &, std::string);
  ~Centroid();
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
  int kppInitialization(const std::function<double(const Data &, const Data &)> &);
  bool LloydsAssignment(const std::function<double(const Data &, const Data &)> &);
  bool reverseAssignment(int, std::string &, int, int, int, int, int, const std::function<double(const Data &, const Data &)> &);
  int updateCentroid();
  int Silhouette(const std::function<double(const Data &, const Data &)> &);

public:
  int begin(std::string &, std::string &, bool, int, int, int, int, int, const std::function<double(const Data &, const Data &)> &);
  Cluster(int, std::string, std::string &);
  ~Cluster();
};

#endif