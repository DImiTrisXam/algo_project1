#ifndef __UTILITIES__
#define __UTILITIES__

#include "../lsh/lshSearch.hpp"
#include <chrono>
#include <string>

bool parseLSHArgs(int, const char **, std::string &, std::string &, std::string &, int &, int &, int &, int &);
bool parseCubeArgs(int, const char **, std::string &, std::string &, std::string &, int &, int &, int &, int &, int &);
bool parseClusterArgs(int, const char **, std::string &, std::string &, std::string &, bool &, std::string &);
int readNumberOfLines(std::string, int &);
int readInputFile(std::string &, HashTable **, int);
std::vector<Data *>* readQueryFile(std::string &);
void printOutputFile(std::ofstream &, const std::string , const std::string &, std::vector<Neighbor> &, std::vector<Neighbor> &,
                     std::vector<std::string> &, std::chrono::nanoseconds, std::chrono::nanoseconds);
bool parseConfigFile(std::string &, int &, int &, int &, int &, int &, int &);

#endif
