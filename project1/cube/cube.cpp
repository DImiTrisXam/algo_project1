#include "../utilities/hypercube.hpp"
#include "../utilities/metrics.hpp"
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

  std::cout << "Input file: \"" << iFile__ << "\"\n"
            << "Query file: \"" << qFile__ << "\"\n"
            << "Output file: \"" << oFile__ << "\"\n"
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
  std::string answer;
  std::vector<Data *> *queries;

  while (true) {
    queries = readQueryFile(qFile__);

    if (!queries) {
      std::cout << "Invalid query file. Exiting program...\n";
      break;
    }

    std::cout << "Printing to output file... ";

    for (const auto query : *queries) {
      auto start = std::chrono::high_resolution_clock::now();
      auto trueDistVec = trueDistanceN(*query, N, cube, euclidianDist);
      auto end = std::chrono::high_resolution_clock::now();

      auto tTrue = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

      // time approximateKNN function
      start = std::chrono::high_resolution_clock::now();
      auto knnVec = approximateKNN(*query, N, cube, M, probes, k, euclidianDist);
      end = std::chrono::high_resolution_clock::now();

      auto tLSH = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

      // approximateRangeSearch
      auto rVec = approximateRangeSearch(*query, R, cube, M, probes, k, euclidianDist);
      printOutputFile(ofile, "Hypercube", query->id, trueDistVec, knnVec, rVec, tLSH, tTrue);
    }

    for (const Data *data : *queries)
      delete data;
    delete queries;

    std::cout << "DONE\nPress X to terminate or Press Y to continue with new query file: ";
    std::cin >> answer;

    if (answer.compare("X") == 0)
      break;
    else if (answer.compare("Y") == 0) {
      std::cout << "Enter path of new query file: ";
      std::cin >> qFile__;
    } else
      break;
  }

  // release hypercube memory
  delete cube;

  return 0;
}
