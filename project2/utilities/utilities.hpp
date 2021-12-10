#ifndef __UTILITIES__
#define __UTILITIES__

#include "hash.hpp"
#include "grid.hpp"
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
void printOutputFile(std::ofstream &, const std::string, const std::string &, std::vector<Neighbor> &, std::vector<Neighbor> &, std::chrono::nanoseconds, std::chrono::nanoseconds);
bool parseConfigFile(std::string &, int &, int &, int &, int &, int &, int &);

#endif
