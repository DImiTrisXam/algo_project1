#include <cstdlib>
#include <fstream>
#include <iostream>

void use(void) {
  // how to use the program
  std::cout << "\nUsage: -i <input file>\n"
            << "\n       -c <configuration file>\n"
            << "\n       -o <output file>\n"
            << "\n       -complete <optional>\n"
            << "\n       -m <method: Classic OR LSH or Hypercube>\n";

  exit(1);
}

int main(int argc, char const *argv[]) {
  std::string iFile__; // input file name
  std::string cFile__; // configuration file name
  std::string oFile__; // output file name
  std::string complete;
  std::string method; // type of method (Classic, LSH or Hypercube)
  int k = 14, M = 10, probes = 2, N = 1, R = 10000;

  if (argc != 1) {
    std::string temp; // helper variable

    for (int i = 0; i < argc - 1; i += 2) { // go through parameters
      if (argv[1][0] != '-') {              // if there is no parameter
        std::cout << "Error: Wrong argument --> " << argv[1] << "\n";
        use();
      }

      switch (argv[1][1]) {
      case 'i':
        iFile__ = argv[2];
        break;
      case 'c':
        cFile__ = argv[2];
        temp = argv[1];
        if (temp.compare("-c") == 0)
          cFile__ = argv[2];
        else if (temp.compare("-complete") == 0)
          complete = argv[2];
        else {
          std::cout << "Error: Wrong argument --> " << argv[1] << "\n";
          use();
        }
        break;
      case 'm':
        method = argv[2];
        break;
      case 'o':
        oFile__ = argv[2];
        break;
      default:
        std::cout << "Error: Wrong argument --> " << argv[1] << "\n";
        use();
      }
      argv += 2; // go to next parameter
    }
  }

  if (iFile__.empty()) {
    std::cout << "Enter path of input file: ";
    std::cin >> iFile__;
  }

  if (cFile__.empty()) {
    std::cout << "Enter path of config file: ";
    std::cin >> cFile__;
  }

  if (oFile__.empty()) {
    std::cout << "Enter path of output file: ";
    std::cin >> oFile__;
  }

  std::cout << "iFile__: \"" << iFile__ << "\"\n"
            << "cFile__: \"" << cFile__ << "\"\n"
            << "oFile__: \"" << oFile__ << "\"\n"
            << "complete: " << complete << "\n"
            << "method: " << method << "\n";

  std::ifstream iFile(iFile__);
  std::ifstream qFile(cFile__);

  return 0;
}
