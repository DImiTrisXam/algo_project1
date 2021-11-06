#include "../utilities/utilities.hpp"
#include <fstream>
#include <iostream>

void use(void) {
  // how to use the program
  std::cout << "\nUsage: -i <input file>\n"
            << "\n       -q <query file>\n"
            << "\n       -k <int>\n"
            << "\n       -M <int>\n"
            << "\n       -probes <int>\n"
            << "\n       -o <output file>\n"
            << "\n       -N <number of nearest>\n"
            << "\n       -R <radius>\n";

  exit(1);
}

int main(int argc, char const *argv[]) {
  std::string iFile__; // input file name
  std::string qFile__; // query file name
  std::string oFile__; // output file name
  int k = -1, M = -1, probes = -1, N = -1, R = -1;

  parseCubeArgs(argc, argv, iFile__, qFile__, oFile__, k, N, R, M, probes);

  std::cout << "iFile__: \"" << iFile__ << "\"\n"
            << "qFile__: \"" << qFile__ << "\"\n"
            << "oFile__: \"" << oFile__ << "\"\n"
            << "k: " << k << "\n"
            << "M: " << M << "\n"
            << "probes: " << probes << "\n"
            << "N: " << N << "\n"
            << "R: " << R << "\n";

  int dim = 0; // dimension of data
  int numOfInputs = readNumberOfLines(iFile__, dim);
  int tableSize = numOfInputs / 8;
  int w = 2; // window for hash table

  auto **tables = new HashTable *[1];

  tables[0] = new HashTable(k, w, dim, tableSize);

  readInputFile(iFile__, tables, 1);                             // put the input in the hash tables
  readQueryFile(qFile__, oFile__, "Hypercube", N, R, tables, 1); // search each query in the tables

  // release hash table memory
  delete tables[0];
  delete[] tables;

  return 0;
}
