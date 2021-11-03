#ifndef __UTILITIES__
#define __UTILITIES__

#include "../lsh/lshSearch.hpp"
#include <string>

bool parseLSHArgs(int, const char **, std::string &, std::string &, std::string &, int &, int &, int &, int &);
bool parseCubeArgs(int, const char **, std::string &, std::string &, std::string &, int &, int &, int &, int &, int &);
bool parseClusterArgs(int, const char **, std::string &, std::string &, std::string &, bool &, std::string &);
int readNumberOfLines(std::string, int &);
int readInputFile(std::string &, HashTable **, int);
int readQueryFile(std::string &, std::string &, const std::string &, int, int, HashTable **, int);
void printOutputFile(std::ofstream &, const std::string &, std::string &, std::vector<double> &, std::vector<Neighbor> &, std::vector<std::string> &, double, double);

#endif
