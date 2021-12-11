#include "../utilities/grid.hpp"
#include "../utilities/metrics.hpp"
#include "../utilities/utilities.hpp"
#include "cubeSearch.hpp"
#include "lshSearch.hpp"
#include <algorithm>
#include <cmath>
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
  int k = -1, L = -1, N = 1, M = -1, probes = -1;
  double delta = -1;
  std::string algorithm, metric;

  parseSearchArgs(argc, argv, iFile__, oFile__, qFile__, algorithm, k, L, M, probes, metric, delta);

  std::cout << "Input file: \"" << iFile__ << "\"\n"
            << "Query file: \"" << qFile__ << "\"\n"
            << "Output file: \"" << oFile__ << "\"\n"
            << "k: " << k << "\n"
            << "L: " << L << "\n"
            << "N: " << N << "\n"
            << "M: " << M << "\n"
            << "probes: " << probes << "\n"
            << "delta: " << delta << "\n"
            << "algorithm: " << algorithm << "\n"
            << "metric: " << metric << "\n";

  int dim = 0; // dimension of data
  int numOfInputs = readNumberOfLines(iFile__, dim);
  int tableSize;
  int w = 2; // window for hash table
  HashTable **tables;
  Grid **grids;
  HashTable *cube;

  if (algorithm.compare("LSH") == 0 || algorithm.compare("Frechet") == 0) {
    tableSize = numOfInputs / 8;
    tables = new HashTable *[L];

    if (algorithm.compare("Frechet") == 0)
      grids = new Grid *[L];

    for (auto i = 0; i < L; i++) {
      tables[i] = new HashTable(k, w, dim, tableSize);

      if (algorithm.compare("Frechet") == 0)
        grids[i] = new Grid(delta);
    }

    std::cout << "numOfInputs: " << numOfInputs << ", dim: " << dim << "\n";

    readInputFile(iFile__, tables, L, algorithm, metric, delta, grids); // put the input in the hash tables
  } else if (algorithm.compare("Hypercube") == 0) {
    tableSize = pow(2, k);
    cube = (HashTable *)new Hypercube(k, w, dim, tableSize);

    readInputFile(iFile__, &cube, 1, algorithm, metric, delta, grids); // put the input in the hypercube
  }

  std::ofstream ofile(oFile__);
  std::string answer;
  std::vector<Data *> *queries;
  double tTrueAver = 0, tLSHAver = 0; // average times
  std::vector<double> approximationFactors;

  while (true) {
    if (algorithm.compare("LSH") == 0 || algorithm.compare("Hypercube") == 0) {
      queries = readQueryFile(qFile__, false);
    } else if (algorithm.compare("Frechet") == 0) {
      queries = readQueryFile(qFile__, true);
    }

    if (!queries) {
      std::cout << "Invalid query file. Exiting program...\n";
      break;
    }

    std::cout << "Printing to output file... ";

    // search each query in the tables
    for (const auto query : *queries) {
      std::vector<Neighbor> trueDistVec;
      std::vector<Neighbor> knnVec;

      // time trueDistanceN function
      auto start = std::chrono::high_resolution_clock::now();

      if (algorithm.compare("LSH") == 0) {
        trueDistVec = trueDistanceN(*query, N, tables, L, euclidianDist);
      } else if (algorithm.compare("Frechet") == 0) {
        trueDistVec = trueDistanceN(*query, N, tables, L, discreteFrechetDist);
      } else if (algorithm.compare("Hypercube") == 0) {
        trueDistVec = trueDistanceN(*query, N, cube, euclidianDist);
      }

      auto end = std::chrono::high_resolution_clock::now();

      tTrueAver += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;

      // time approximateKNN function
      start = std::chrono::high_resolution_clock::now();

      if (algorithm.compare("LSH") == 0) {
        knnVec = approximateKNN(*query, N, tables, grids, L, metric, euclidianDist);
      } else if (algorithm.compare("Frechet") == 0) {
        knnVec = approximateKNN(*query, N, tables, grids, L, metric, discreteFrechetDist);
      } else if (algorithm.compare("Hypercube") == 0) {
        knnVec = approximateKNN(*query, N, cube, M, probes, k, euclidianDist);
      }

      end = std::chrono::high_resolution_clock::now();

      tLSHAver += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;

      approximationFactors.push_back(knnVec[knnVec.size() - 1].dist / trueDistVec[trueDistVec.size() - 1].dist);

      printOutputFile(ofile, algorithm, metric, query->id, trueDistVec, knnVec);
    }

    tTrueAver = tTrueAver / numOfInputs;
    tLSHAver = tLSHAver / numOfInputs;

    // Maximum Approximation Factor
    auto MAF = *std::max_element(approximationFactors.begin(), approximationFactors.end());

    printStatistics(ofile, tLSHAver, tTrueAver, MAF);

    // for (const Data *data : *queries)
    //   delete data;
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

  if (algorithm.compare("LSH") == 0 || algorithm.compare("Frechet") == 0) {
    if (algorithm.compare("Frechet") == 0) {
      // release grid memory
      for (auto i = 0; i < L; i++)
        delete grids[i];
      delete[] grids;
    }

    // release hash table memory
    // for (auto i = 0; i < L; i++)
    //   delete tables[i];
    delete[] tables;
  } else if (algorithm.compare("Hypercube") == 0) {
    // release hypercube memory
    delete cube;
  }

  return 0;
}
