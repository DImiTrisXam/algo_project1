#include "../utilities/hash.hpp"
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
  /*std::string iFile__; // input file name
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

  int numOfInputs = readNumberOfLines(iFile__);
  unsigned int tableSize = numOfInputs / 8;

  // std::cout << "Number of lines in input file: " << numOfInputs << "\n";

  auto **tables = new HashTable *[L];

  for (size_t i = 0; i < L; i++) {
    tables[i] = new HashTable(tableSize);
  }

  readInputFile(iFile__, tables, L); // put the input in the hash tables

  std::ifstream qFile(qFile__);

  // release hash table memory
  for (size_t i = 0; i < L; i++) {
    delete tables[i];
  }
  delete[] tables;

  return 0;
  */

 
 HashTable table(4, 2, 8, 10);

  //std::ifstream file("test_data");
  std::ifstream file;
  file.open("./test", std::ifstream::in);

  std::cout << file.is_open() << std::endl;
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string id;
    std::vector<float> vec;
    int temp;

    ss >> id;

    while (ss >> temp) {
      vec.push_back(temp);
    }

    table.add(vec, id);
  }

  table.PRINT();

  return 0; 
 
}
