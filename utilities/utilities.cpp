#include "utilities.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

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
      complete = argv[++i];
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

int readInputFile(std::string name, HashTable **tables, int L) {
  std::ifstream file(name);
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    Data *data;

    std::string id;
    int temp;

    ss >> id;

    data = new Data(id);

    while (ss >> temp)
      data->value.push_back(temp);

    for (size_t i = 0; i < L; i++)
      tables[i]->insert(*data);
  }
}

int readQueryFile(std::string name, HashTable **tables, int L) {
  std::ifstream file(name);
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream ss(line);

    Data *data;

    std::string id;
    int temp;

    ss >> id;

    data = new Data(id);

    while (ss >> temp)
      data->value.push_back(temp);
  }
}