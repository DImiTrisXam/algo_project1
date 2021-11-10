#include "utilities.hpp"
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

bool parseLSHArgs(int argc, const char **argv, std::string &input_file, std::string &query_file, std::string &output_file, int &k, int &L, int &N, int &R) {
  bool validArgs = true;
  int i;

  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], "-i")) {
      input_file = argv[++i];
    } else if (!strcmp(argv[i], "-q")) {
      query_file = argv[++i];
    } else if (!strcmp(argv[i], "-o")) {
      output_file = argv[++i];
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
    } else if (!strcmp(argv[i], "-N")) {
      if (tryParseArgInt(N, argv[++i], "-N") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-R")) {
      if (tryParseArgInt(R, argv[++i], "-R") == false) {
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

  // default parameters
  if (k == -1)
    k = 4;
  if (L == -1)
    L = 5;
  if (N == -1)
    N = 1;
  if (R == -1)
    R = 10000;

  return true;
}

bool parseCubeArgs(int argc, const char **argv, std::string &input_file, std::string &query_file, std::string &output_file, int &k, int &N, int &R, int &M, int &probes) {
  bool validArgs = true;
  int i;

  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], "-i")) {
      input_file = argv[++i];
    } else if (!strcmp(argv[i], "-q")) {
      query_file = argv[++i];
    } else if (!strcmp(argv[i], "-o")) {
      output_file = argv[++i];
    } else if (!strcmp(argv[i], "-k")) {
      if (tryParseArgInt(k, argv[++i], "-k") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-M")) {
      if (tryParseArgInt(M, argv[++i], "-M") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-N")) {
      if (tryParseArgInt(N, argv[++i], "-N") == false) {
        validArgs = false;
        i--;
      }
    } else if (!strcmp(argv[i], "-R")) {
      if (tryParseArgInt(R, argv[++i], "-R") == false) {
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

  // default parameters
  if (k == -1)
    k = 14;
  if (M == -1)
    M = 10;
  if (N == -1)
    N = 1;
  if (R == -1)
    R = 10000;
  if (probes == -1)
    probes = 2;

  return true;
}

bool parseClusterArgs(int argc, const char **argv, std::string &input_file, std::string &output_file, std::string &config_file, bool &complete, std::string &method) {
  bool validArgs = true;
  int i;

  for (i = 1; i < argc - 1; i++) {
    if (!strcmp(argv[i], "-i")) {
      input_file = argv[++i];
    } else if (!strcmp(argv[i], "-c")) {
      config_file = argv[++i];
    } else if (!strcmp(argv[i], "-o")) {
      output_file = argv[++i];
    } else if (!strcmp(argv[i], "-m")) {
      method = argv[++i];
    } else if (!strcmp(argv[i], "-complete")) {
      complete = true;
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
      int temp;

      ss >> id;

      while (ss >> temp)
        dim++; // increase dimension of data
    }

    lines++;
  }

  return lines;
}

int readInputFile(std::string &name, HashTable **tables, int L) {
  std::ifstream file(name);
  std::string line;

  std::cout << "Processing input file... ";

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string id;
    std::vector<float> vec;
    float temp;

    ss >> id;

    while (ss >> temp)
      vec.push_back(temp);

    for (size_t i = 0; i < L; i++)
      tables[i]->add(vec, id);
  }

  std::cout << "DONE\n";

  return 0;
}

std::vector<Data *>* readQueryFile(std::string &qfile_) {
  std::ifstream qfile(qfile_);
  // std::ofstream file;
  // file.open(file_, std::ios_base::app);
  std::string line;

  std::cout << "Processing query file and printing to output file... ";

  std::vector<Data *> *queries = new std::vector<Data *>;

  while (std::getline(qfile, line)) {
    std::istringstream ss(line);

    std::string id;
    std::vector<float> vec;
    float temp;

    ss >> id;

    while (ss >> temp)
      vec.push_back(temp);

    Data *query = new Data(vec, id);

    queries->push_back(query);
  }

  return queries;
}

void printOutputFile(std::ofstream &file, const std::string lshORcube, const std::string &qid, std::vector<Neighbor> &trueDistVec,
                     std::vector<Neighbor> &knnVec, std::vector<std::string> &rVec, std::chrono::nanoseconds tLSH, std::chrono::nanoseconds tTrue) {
  file << "Query: " << qid << '\n';

  for (auto i = 0; i < knnVec.size(); i++) {
    file << "Nearest neighbor-" << i + 1 << ": " << knnVec[i].id << '\n'
         << "distance" << lshORcube << ": " << knnVec[i].dist << '\n'
         << "distanceTrue: " << trueDistVec[i].dist << '\n';
  }
  // multiply nanoseconds with 10^-9 to print seconds
  file << "tLSH: " << tLSH.count() * 1e-9 << " seconds\n"
       << "tTrue: " << tTrue.count() * 1e-9 << " seconds\n"
       << "R-near neighbors:\n";

  for (const auto &id : rVec)
    file << id << '\n';
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