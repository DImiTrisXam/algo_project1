#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
  int k = 4, L = 5, N = 1, R = 10000;

  if (argc != 1) {
    for (int i = 0; i < argc - 1; i += 2) { // go through parameters
      if (argv[1][0] != '-') {              // if there is no parameter
        std::cout << "Error: Wrong argument --> " << argv[1] << "\n";
        use();
      }

      switch (argv[1][1]) {
      case 'i':
        iFile__ = argv[2];
        break;
      case 'q':
        qFile__ = argv[2];
        break;
      case 'k':
        k = atoi(argv[2]);
        break;
      case 'L':
        L = atoi(argv[2]);
        break;
      case 'o':
        oFile__ = argv[2];
        break;
      case 'N':
        N = atoi(argv[2]);
        break;
      case 'R':
        R = atoi(argv[2]);
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

  if (qFile__.empty()) {
    std::cout << "Enter path of query file: ";
    std::cin >> qFile__;
  }

  if (oFile__.empty()) {
    std::cout << "Enter path of output file: ";
    std::cin >> oFile__;
  }

  std::cout << "iFile__: \"" << iFile__ << "\"\n"
            << "qFile__: \"" << qFile__ << "\"\n"
            << "oFile__: \"" << oFile__ << "\"\n"
            << "k: " << k << "\n"
            << "L: " << L << "\n"
            << "N: " << N << "\n"
            << "R: " << R << "\n";

  std::ifstream iFile(iFile__);
  std::ifstream qFile(qFile__);

  std::string line;

  while (std::getline(iFile, line)) {
    std::istringstream ss(line);

    std::string name;
    int var1, var2, var3;

    ss >> name >> var1 >> var2 >> var3;
  }

  return 0;
}
