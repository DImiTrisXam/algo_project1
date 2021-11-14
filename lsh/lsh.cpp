#include "../utilities/utilities.hpp"
#include "../utilities/metrics.hpp"
#include "lshSearch.hpp"
#include <fstream>
#include <iostream>
#include <vector>

static void use(void) {
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

  readInputFile(iFile__, tables, L); // put the input in the hash tables

  std::ofstream ofile(oFile__);

  std::vector<Data *> *queries = readQueryFile(qFile__);

  std::cout << "Printing to output file... ";

  // search each query in the tables
  for (const auto query : *queries) {
    auto start = std::chrono::high_resolution_clock::now();
    auto trueDistVec = trueDistanceN(*query, N, tables, L, euclidianDist);
    auto end = std::chrono::high_resolution_clock::now();

    auto tTrue = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // for (const auto &p : trueDistVec) {
    //   std::cout << p.id << ", " << p.dist << "  ";
    // }
    // std::cout << "\n";

    // time approximateKNN function
    start = std::chrono::high_resolution_clock::now();
    auto knnVec = approximateKNN(*query, N, tables, L, euclidianDist);
    end = std::chrono::high_resolution_clock::now();

    auto tLSH = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // for (const auto &p : knnVec) {
    //   std::cout << p.id << ", " << p.dist << "  ";
    // }
    // std::cout << "\n";

    // approximateRangeSearch works
    auto rVec = approximateRangeSearch(*query, R, tables, L, euclidianDist);
    printOutputFile(ofile, "LSH", query->id, trueDistVec, knnVec, rVec, tLSH, tTrue);
  }

  std::cout << "DONE\n";

  for (const Data *data : *queries) {
    delete data;
  }
  delete queries;

  // release hash table memory
  for (size_t i = 0; i < L; i++)
    delete tables[i];
  delete[] tables;

  return 0;
}
