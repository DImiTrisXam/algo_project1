#ifndef __UTILITIES__
#define __UTILITIES__

#include "../utilities/HashTable.hpp"
#include <fstream>
#include <string>
#include <vector>

bool parseLSHArgs(int, const char **, std::string &, std::string &, std::string &, int &, int &, int &, int &);
bool parseCubeArgs(int, const char **, std::string &, std::string &, std::string &, int &, int &, int &, int &, int &);
bool parseClusterArgs(int, const char **, std::string &, std::string &, std::string &, bool &, std::string &);
int readNumberOfLines(std::string, int &);
int readInputFile(std::string, HashTable **, int);
int readQueryFile(std::string, HashTable **, int);

#endif
