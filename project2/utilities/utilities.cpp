#include "utilities.hpp"
#include "curve.hpp"
#include "grid.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

//================================================================================
//================================== PARSE ARGS ==================================
static bool tryParseArgInt(int &arg, const char *str, const char *name) {
  if (arg != -1) {
    std::cout << name << " passed more than once" << std::endl;
    return false;
  }

  try {
    arg = std::stoi(str);
    if (arg < 0) {
      arg = -2;
      std::cout << "Invalid value after " << name << std::endl;
    }
  } catch (std::invalid_argument &e) {
    std::cout << "Expected int after " << name << std::endl;
    return false;
  } catch (std::out_of_range &e) {
    std::cout << "Too large number after " << name << std::endl;
    return false;
  }

  return true;
}

static bool tryParseArgDouble(double &arg, const char *str, const char *name) {
  if (arg != -1) {
    std::cout << name << " passed more than once" << std::endl;
    return false;
  }

  try {
    arg = std::stod(str);
    if (arg < 0) {
      arg = -2;
      std::cout << "Invalid value after " << name << std::endl;
    }
  } catch (std::invalid_argument &e) {
    std::cout << "Expected double after " << name << std::endl;
    return false;
  } catch (std::out_of_range &e) {
    std::cout << "Too large number after " << name << std::endl;
    return false;
  }

  return true;
}

bool parseClusterArgs(int argc, const char **argv, std::string &input_file, std::string &output_file, std::string &config_file,
                      std::string &update, std::string &assignment, bool &complete, bool &silhouette) {
  bool validArgs = true;
  int i;

  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], "-i")) {
      input_file = argv[++i];
    } else if (!strcmp(argv[i], "-c")) {
      config_file = argv[++i];
    } else if (!strcmp(argv[i], "-o")) {
      output_file = argv[++i];
    } else if (!strcmp(argv[i], "-update")) {
      update = argv[++i];
      update += " ";
      update += argv[++i];
    } else if (!strcmp(argv[i], "-assignment")) {
      assignment = argv[++i];
    } else if (!strcmp(argv[i], "-complete")) {
      complete = true;
    } else if (!strcmp(argv[i], "-silhouette")) {
      silhouette = true;
    } else {
      std::cout << "Unexpected parameter " << argv[i] << std::endl;
      validArgs = false;
    }
  }

  if (i != argc) {
    std::cout << "Unexpected parameter " << argv[i] << std::endl;
    validArgs = false;
  }
  if (!validArgs)
    return false;

  if (input_file.empty()) {
    std::cout << "Enter path of input file: ";
    std::cin >> input_file;
  }
  if (output_file.empty()) {
    std::cout << "Enter path of output file: ";
    std::cin >> output_file;
  }
  if (config_file.empty()) {
    std::cout << "Enter path of configuration file: ";
    std::cin >> config_file;
  }

  return true;
}

bool parseSearchArgs(int argc, const char **argv, std::string &input_file, std::string &output_file, std::string &query_file,
                     std::string &algorithm, int &k, int &L, int &M, int &probes, std::string &metric, double &delta) {
  bool validArgs = true;
  int i;

  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], "-i")) {
      input_file = argv[++i];
    } else if (!strcmp(argv[i], "-q")) {
      query_file = argv[++i];
    } else if (!strcmp(argv[i], "-o")) {
      output_file = argv[++i];
    } else if (!strcmp(argv[i], "-metric")) {
      metric = argv[++i];
    } else if (!strcmp(argv[i], "-delta")) {
      if (tryParseArgDouble(delta, argv[++i], "-delta") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-algorithm")) {
      algorithm = argv[++i];
    } else if (!strcmp(argv[i], "-k")) {
      if (tryParseArgInt(k, argv[++i], "-k") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-L")) {
      if (tryParseArgInt(L, argv[++i], "-L") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-M")) {
      if (tryParseArgInt(M, argv[++i], "-M") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-probes")) {
      if (tryParseArgInt(probes, argv[++i], "-probes") == false) {
        validArgs = false;
        i--;
      }
    } else {
      std::cout << "Unexpected parameter " << argv[i] << std::endl;
      validArgs = false;
    }
  }

  if (i != argc) {
    std::cout << "Unexpected parameter " << argv[i] << std::endl;
    validArgs = false;
  }
  if (!validArgs)
    return false;

  if (input_file.empty()) {
    std::cout << "Enter path of input file: ";
    std::cin >> input_file;
  }
  if (output_file.empty()) {
    std::cout << "Enter path of output file: ";
    std::cin >> output_file;
  }
  if (query_file.empty()) {
    std::cout << "Enter path of query file: ";
    std::cin >> query_file;
  }

  //default args
  if (algorithm.compare("LSH")) {
    if (k == -1)
      k = 4;
    if (L == -1)
      L = 5;
  } else if (algorithm.compare("Hypercube")) {
    if (k == -1)
      k = 14;
    if (M == -1)
      M = 10;
    if (probes == -1)
      probes = 2;
  }

  return true;
}
//================================================================================
//================================== FILE UTILS ==================================

int readNumberOfLines(std::string name, int &dim) {
  int lines = 0;
  std::string line;
  std::ifstream file(name);

  while (std::getline(file, line)) {
    if (lines == 0) { // only for first line
      std::istringstream ss(line);
      std::string id;
      float temp;

      ss >> id;

      while (ss >> temp)
        dim++; // increase dimension of data
    }

    lines++;
  }

  return lines;
}

int readInputFile(std::string &name, HashTable **tables, int L, std::string &algorithm, std::string &metric, double delta, Grid **grids) {
  std::ifstream file(name);
  std::string line;
  std::vector<float> tVec;

  std::cout << "Processing input file... ";

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string id;
    std::vector<float> vec;
    float temp;

    ss >> id;

    // std::cout << "id: " << id << "\n";

    if (algorithm.compare("LSH") == 0 || algorithm.compare("Hypercube") == 0) {
      while (ss >> temp)
        vec.push_back(temp);

      for (auto i = 0; i < L; i++)
        tables[i]->add(vec, id);
    } else {
      while (ss >> temp)
        vec.push_back(temp);

      if (tVec.empty()) { // do it once
        for (auto i = 0; i < vec.size(); i++)
          tVec.push_back(i + 1);
      }

      for (auto i = 0; i < L; i++) {
        Curve c(vec, tVec, id);

        if (metric.compare("discrete") == 0) {
          grids[i]->snapTo2DGrid(c); // snap curve
          c.collapseGridToVector();
          c.padding();
        } else {
          float averageVariation = 0;

          for (auto i = 0; i < vec.size() - 1; i++)
            averageVariation += abs(c.vec[i + 1] - c.vec[i]);

          averageVariation = averageVariation / vec.size();
          float epsilon = (averageVariation * 160) / 100;

          c.filter(epsilon);
          grids[i]->snapTo1DGrid(c); // snap curve
          c.getMinimaMaxima();
          c.padding();
        }

        tables[i]->add(c.vec, c.tVec, c.key, c.id);
      }
    }
  }

  std::cout << "DONE\n";

  return 0;
}

std::vector<Data *> *readQueryFile(std::string &qfile_, bool curve) {
  std::ifstream qfile(qfile_);

  if (!qfile.good())
    return nullptr;

  std::string line;

  std::cout << "Processing query file... ";

  auto queries = new std::vector<Data *>;

  while (std::getline(qfile, line)) {
    std::istringstream ss(line);

    std::string id;
    std::vector<float> vec;
    std::vector<float> tVec;
    float temp;
    Data *query;

    ss >> id;

    if (!curve) {
      while (ss >> temp)
        vec.push_back(temp);

      query = new Data(vec, id);
    } else {
      while (ss >> temp)
        vec.push_back(temp);

      for (auto i = 0; i < vec.size(); i++)
        tVec.push_back(i + 1);

      try {
        query = (Data *)new Curve(vec, tVec, id);
      } catch (const std::bad_alloc &e) {
        std::cout << "Allocation failed in readQueryFile: " << e.what() << '\n';
      }
    }

    queries->push_back(query);
  }

  std::cout << "DONE\n";

  return queries;
}

void printOutputFile(std::ofstream &file, std::string &algorithm, std::string &metric, const std::string &qid, std::vector<Neighbor> &trueDistVec,
                     std::vector<Neighbor> &knnVec) {
  file << "Query: " << qid << '\n'
       << "Algorithm: ";

  if (algorithm.compare("LSH") == 0) {
    file << "LSH_Vector" << '\n';
  } else if (algorithm.compare("Hypercube") == 0) {
    file << "Hypercube" << '\n';
  } else {
    if (metric.compare("discrete") == 0) {
      file << "LSH_Frechet_Discrete" << '\n';
    } else {
      file << "LSH_Frechet_Continuous" << '\n';
    }
  }

  for (auto i = 0; i < knnVec.size(); i++) {
    file << "Approximate Nearest neighbor: " << knnVec[i].id << '\n'
         << "distanceApproximate: " << knnVec[i].dist << '\n'
         << "distanceTrue: " << trueDistVec[i].dist << '\n';
  }
}

void printStatistics(std::ofstream &file, double tLSH, double tTrue, double MAF) {
  // multiply nanoseconds with 10^-9 to print seconds
  file << "\ntApproximateAverage: " << tLSH << " seconds\n"
       << "tTrueAverage: " << tTrue << " seconds\n"
       << "MAF: " << MAF << "\n";
}

bool parseConfigFile(std::string &name, int &K, int &L, int &k, int &M, int &d, int &probes) {
  std::ifstream file(name);
  std::string line;
  bool validArgs = true;

  std::cout << "Processing config file... ";

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string parameter;
    std::string value;

    ss >> parameter;
    ss >> value;

    if (!parameter.compare("number_of_clusters:")) {
      if (tryParseArgInt(K, value.c_str(), "number_of_clusters:") == false) {
        validArgs = false;
      }
    } else if (!parameter.compare("number_of_vector_hash_tables:")) {
      if (tryParseArgInt(L, value.c_str(), "number_of_vector_hash_tables:") == false) {
        validArgs = false;
      }
    } else if (!parameter.compare("number_of_vector_hash_functions:")) {
      if (tryParseArgInt(k, value.c_str(), "number_of_vector_hash_functions:") == false) {
        validArgs = false;
      }
    } else if (!parameter.compare("max_number_M_hypercube:")) {
      if (tryParseArgInt(M, value.c_str(), "max_number_M_hypercube:") == false) {
        validArgs = false;
      }
    } else if (!parameter.compare("number_of_hypercube_dimensions:")) {
      if (tryParseArgInt(d, value.c_str(), "number_of_hypercube_dimensions:") == false) {
        validArgs = false;
      }
    } else if (!parameter.compare("number_of_probes:")) {
      if (tryParseArgInt(probes, value.c_str(), "number_of_probes:") == false) {
        validArgs = false;
      }
    } else {
      std::cout << "Unexpected parameter " << parameter << std::endl;
      validArgs = false;
    }
  }

  std::cout << "DONE\n";

  if (!validArgs)
    return false;

  // default parameters
  if (L == -1)
    L = 3;
  if (k == -1)
    k = 4;
  if (M == -1)
    M = 10;
  if (d == -1)
    d = 3;
  if (probes == -1)
    probes = 2;

  return true;
}