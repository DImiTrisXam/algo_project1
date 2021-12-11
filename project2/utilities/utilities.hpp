#ifndef __UTILITIES__
#define __UTILITIES__

#include "grid.hpp"
#include "hash.hpp"
#include <chrono>
#include <string>

struct Neighbor {
  std::string id;
  double dist; // distance (key for priority queue)
};

bool parseSearchArgs(int, const char **, std::string &, std::string &, std::string &, std::string &, int &, int &, int &, int &, std::string &, double &);
bool parseClusterArgs(int, const char **, std::string &, std::string &, std::string &, std::string &, std::string &, bool &, bool &);
int readNumberOfLines(std::string, int &);
int readInputFile(std::string &, HashTable **, int, std::string &, std::string &, double, Grid **);
std::vector<Data *> *readQueryFile(std::string &, bool);
void printOutputFile(std::ofstream &, std::string &, std::string &, const std::string &, std::vector<Neighbor> &, std::vector<Neighbor> &);
void printStatistics(std::ofstream &, double, double, double);
bool parseConfigFile(std::string &, int &, int &, int &, int &, int &, int &);

#endif
