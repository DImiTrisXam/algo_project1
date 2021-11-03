#include "../utilities/utilities.hpp"
#include "lshSearch.hpp"
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

  std::cout << "Input file: \"" << iFile__ << "\"\n"
            << "Query file: \"" << qFile__ << "\"\n"
            << "Output file: \"" << oFile__ << "\"\n"
            << "k: " << k << "\n"
            << "L: " << L << "\n"
            << "N: " << N << "\n"
            << "R: " << R << "\n";

  int dim = 0; // dimension of data
  int numOfInputs = readNumberOfLines(iFile__, dim);
  int tableSize = numOfInputs / 8;
  int w = 2; // window for hash table

  auto **tables = new HashTable *[L];

  for (size_t i = 0; i < L; i++) {
    tables[i] = new HashTable(k, w, dim, tableSize);
  }

  std::cout << "Processing input file... ";
  readInputFile(iFile__, tables, L); // put the input in the hash tables
  std::cout << "DONE\nProcessing query file... ";
  readQueryFile(qFile__, oFile__, "LSH", N, R, tables, L); // search each query in the tables
  std::cout << "DONE\n";

  // release hash table memory
  for (size_t i = 0; i < L; i++)
    delete tables[i];
  delete[] tables;

  return 0;
}
