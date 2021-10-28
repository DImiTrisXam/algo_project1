#include "../utilities/utilities.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

void use(void) {
  // how to use the program
  std::cout << "\nUsage: -i <input file>\n"
            << "\n       -c <configuration file>\n"
            << "\n       -o <output file>\n"
            << "\n       -complete <optional>\n"
            << "\n       -m <method: Classic OR LSH or Hypercube>\n";

  exit(1);
}

int main(int argc, char const *argv[]) {
  std::string iFile__; // input file name
  std::string cFile__; // configuration file name
  std::string oFile__; // output file name
  bool complete;
  std::string method; // type of method (Classic, LSH or Hypercube)

  parseClusterArgs(argc, argv, iFile__, oFile__, cFile__, complete, method);

  std::cout << "iFile__: \"" << iFile__ << "\"\n"
            << "cFile__: \"" << cFile__ << "\"\n"
            << "oFile__: \"" << oFile__ << "\"\n"
            << "complete: " << complete << "\n"
            << "method: " << method << "\n";

  std::ifstream iFile(iFile__);
  std::ifstream qFile(cFile__);

  return 0;
}
