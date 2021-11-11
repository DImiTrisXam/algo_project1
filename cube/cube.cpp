#include "../utilities/hypercube.hpp"
#include "../utilities/utilities.hpp"
#include "cubeSearch.hpp"
#include <cmath>
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
  int tableSize = pow(2, k);
  int w = 2; // window for hash table

  HashTable *cube = (HashTable *)new Hypercube(k, w, dim, tableSize);

  readInputFile(iFile__, &cube, 1); // put the input in the hypercube

  //cube->PRINT();

  std::ofstream ofile(oFile__);

  std::vector<Data *> *queries = readQueryFile(qFile__);

  std::cout << "Printing to output file... ";

  for (const auto query : *queries) {
    auto start = std::chrono::high_resolution_clock::now();
    auto trueDistVec = trueDistanceN(*query, N, cube);
    auto end = std::chrono::high_resolution_clock::now();

    auto tTrue = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // for (const auto &p : trueDistVec) {
    //   std::cout << p.id << ", " << p.dist << "  ";
    // }
    // std::cout << "\n";

    // time approximateKNN function
    start = std::chrono::high_resolution_clock::now();
    auto knnVec = approximateKNN(*query, N, cube, M, probes, k);
    end = std::chrono::high_resolution_clock::now();

    auto tLSH = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // for (const auto &p : knnVec) {
    //   std::cout << p.id << ", " << p.dist << "  ";
    // }
    // std::cout << "\n";

    // approximateRangeSearch works
    auto rVec = approximateRangeSearch(*query, R, cube, M, probes, k);
    printOutputFile(ofile, "Hypercube", query->id, trueDistVec, knnVec, rVec, tLSH, tTrue);
  }
  
  std::cout << "DONE\n";

  // release hypercube memory
  delete cube;

  return 0;
}
