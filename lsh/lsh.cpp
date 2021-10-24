#include "../utilities/utilities.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void use(void) {
  // how to use the program
  std::cout << "\nUsage: -i <input file>\n"
            << "\n       -q <query file>\n"
            << "\n       -k <int>\n"
            << "\n       -L <int>\n"
            << "\n       -o <output file>\n"
            << "\n       -N <number of nearest>\n"
            << "\n       -R <radius>\n";

  exit(1);
}

int main(int argc, char const *argv[]) {
  std::string iFile__; // input file name
  std::string qFile__; // query file name
  std::string oFile__; // output file name
  int k = -1, L = -1, N = -1, R = -1;

  parseLSHArgs(argc, argv, iFile__, qFile__, oFile__, k, L, N, R);

  std::cout << "iFile__: \"" << iFile__ << "\"\n"
            << "qFile__: \"" << qFile__ << "\"\n"
            << "oFile__: \"" << oFile__ << "\"\n"
            << "k: " << k << "\n"
            << "L: " << L << "\n"
            << "N: " << N << "\n"
            << "R: " << R << "\n";

  std::ifstream iFile(iFile__);
  // std::ifstream qFile(qFile__);

  std::string line;

  while (std::getline(iFile, line)) {
    std::istringstream ss(line);

    std::string name;
    std::vector<int> vec;
    int temp;

    ss >> name;

    while (ss >> temp) {
      vec.push_back(temp);
    }

    std::cout << "entry: \"" << name << "\"";
    
    for (auto entry : vec) {
      std::cout << " " << entry;
    }
    std::cout << "\n";
  }

  return 0;
}
