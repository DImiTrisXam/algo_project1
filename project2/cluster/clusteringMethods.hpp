#ifndef __CLUSTER__
#define __CLUSTER__

#include "../utilities/curve.hpp"
#include <chrono>
#include <unordered_map>
#include <vector>

struct Centroid : public Curve {
  std::vector<float> vecSum; // sum of all points in cluster of centroid
  std::vector<int> indexes;  // indexes of all the points in cluster of centroid
  double silhouette;         // silhouette of centroid's cluster

  Centroid(std::vector<float> &, std::vector<float> &, std::string);
  ~Centroid();
};

class Cluster {
  std::vector<Data *> points;
  std::unordered_map<std::string, size_t> idToIndexMap;
  std::vector<Centroid *> centroids;
  int K;                    // number of clusters
  std::string assignMethod; // method of assignment (Classic, LSH, Hypercube, LSH_Frechet)
  std::string updateMethod; // method of assignment (Mean Frechet, Mean Vector)
  double overallSilhouette;
  bool complete, silhouette; // for printing

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
  int begin(std::string &, std::string &, int, int, int, int, int, const std::function<double(const Data &, const Data &)> &);
  Cluster(int, std::string, std::string, std::string &, bool, bool);
  ~Cluster();
};

#endif