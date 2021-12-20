#include "../utilities/metrics.hpp"
#include "../utilities/utilities.hpp"
#include "clusteringMethods.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

// void use(void) {
//   // how to use the program
//   std::cout << "\nUsage: -i <input file>\n"
//             << "\n       -c <configuration file>\n"
//             << "\n       -o <output file>\n"
//             << "\n       -complete <optional>\n"
//             << "\n       -m <method: Classic OR LSH or Hypercube>\n";

//   exit(1);
// }

int main(int argc, char const *argv[]) {
  std::string iFile__; // input file name
  std::string cFile__; // configuration file name
  std::string oFile__; // output file name
  bool complete, silhouette;
  std::string update;     // type of update method (Mean Frechet or Mean Vector)
  std::string assignment; // type of assignment method (Classic, LSH, Hypercube, LSH_Frechet)

  parseClusterArgs(argc, argv, iFile__, oFile__, cFile__, update, assignment, complete, silhouette);

  std::cout << "Input file: \"" << iFile__ << "\"\n"
            << "Config file: \"" << cFile__ << "\"\n"
            << "Output file: \"" << oFile__ << "\"\n"
            << "update: " << update << "\n"
            << "assignment: " << assignment << "\n"
            << "complete: " << complete << "\n"
            << "silhouette: " << silhouette << "\n\n";

  int K = -1, L = -1, k = -1, M = -1, d = -1, probes = -1;

  parseConfigFile(cFile__, K, L, k, M, d, probes);

  std::cout << "\nnumber_of_clusters: " << K << "\n"
            << "number_of_vector_hash_tables: " << L << "\n"
            << "number_of_vector_hash_functions: " << k << "\n"
            << "max_number_M_hypercube: " << M << "\n"
            << "number_of_hypercube_dimensions: " << d << "\n"
            << "number_of_probes: " << probes << "\n\n";

  Cluster cl = Cluster(K, assignment, update, iFile__, complete, silhouette);
  
  if (update.compare("Mean Frechet") == 0) { // if curve representation
    if (cl.begin(oFile__, iFile__, L, k, M, d, probes, discreteFrechetDist) == -1) {
      std::cout << "\nERROR: Wrong combination of update and assignment methods\n";
      return -1;
    }
  } else { // if vector representation
    if (cl.begin(oFile__, iFile__, L, k, M, d, probes, euclidianDist) == -1) {
      std::cout << "\nERROR: Wrong combination of update and assignment methods\n";
      return -1;
    }
  }

  return 0;
}
