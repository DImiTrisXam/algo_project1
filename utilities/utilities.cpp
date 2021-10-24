#include "utilities.hpp"

//================================================================================
//================================== PARSE ARGS =======================================
static bool tryParseArgInt(int &arg, char *str, const char *name){
    if(arg != -1){
        cout << name << " passed more than once" << endl;
        return false;
    }

    try{
        arg = stoi(str);
        if(arg < 0){
            arg = -2;
            cout << "Invalid value after " << name << endl;
        }
    }
    catch(invalid_argument& e){
        cout << "Expected int after " << name << endl;
        return false;
    }
    catch(out_of_range& e){
        cout << "Too large number after " << name << endl;
        return false;
    }

    return true;
}

bool parseLSHArgs(int argc, char **argv, string &input_file, string &query_file, string &output_file, int &k, int &L, int &N, int &R){
    bool validArgs = true;

    for(int i=1; i<argc-1; i++){
        if(!strcmp(argv[i], "-i")){
            input_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-q")){
            query_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-o")){
            output_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-k")){
            if(tryParseArgInt(k, argv[++i], "-k") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-L")){
            if(tryParseArgInt(k, argv[++i], "-L") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-N")){
            if(tryParseArgInt(k, argv[++i], "-N") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-R")){
            if(tryParseArgInt(k, argv[++i], "-R") == false){
                validArgs = false;
                i--;
            }
        }
        else{
            cout << "Unexpected parameter " << argv[i] << endl;
            validArgs = false;
        }
    }
    if (i != argc){
        cout << "Unexpected parameter " << argv[i] << endl;
        validArgs = false;
    }
    if(!validArgs)
        return false;

    
    if (input_file.empty()){
        std::cout << "Enter path of input file: ";
        std::cin >> input_file;
    }
    if (output_file.empty()){
        std::cout << "Enter path of output file: ";
        std::cin >> output_file;
    }
    if (query_file.empty()){
        std::cout << "Enter path of query file: ";
        std::cin >> query_file;
    }
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

bool parseCubeArgs(int argc, char **argv, string &input_file, string &query_file, string &output_file, int &k, int &N, int &R, int &M, int &probes){
    bool validArgs = true;

    for(int i=1; i<argc-1; i++){
        if(!strcmp(argv[i], "-i")){
            input_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-q")){
            query_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-o")){
            output_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-k")){
            if(tryParseArgInt(k, argv[++i], "-k") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-M")){
            if(tryParseArgInt(k, argv[++i], "-M") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-N")){
            if(tryParseArgInt(k, argv[++i], "-N") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-R")){
            if(tryParseArgInt(k, argv[++i], "-R") == false){
                validArgs = false;
                i--;
            }
        }
        else if(!strcmp(argv[i], "-probes")){
            if(tryParseArgInt(k, argv[++i], "-probes") == false){
                validArgs = false;
                i--;
            }
        }
        else{
            cout << "Unexpected parameter " << argv[i] << endl;
            validArgs = false;
        }
    }
    if (i != argc){
        cout << "Unexpected parameter " << argv[i] << endl;
        validArgs = false;
    }
    if(!validArgs)
        return false;

    
    if (input_file.empty()){
        std::cout << "Enter path of input file: ";
        std::cin >> input_file;
    }
    if (output_file.empty()){
        std::cout << "Enter path of output file: ";
        std::cin >> output_file;
    }
    if (query_file.empty()){
        std::cout << "Enter path of query file: ";
        std::cin >> query_file;
    }
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

bool parseClusterArgs(int argc, char **argv, string &input_file, string &output_file, string &config_file, bool &complete, string &method){
    bool validArgs = true;

    for(int i=1; i<argc-1; i++){
        if(!strcmp(argv[i], "-i")){
            input_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-c")){
            config_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-o")){
            output_file = argv[++i];
        }
        else if(!strcmp(argv[i], "-m")){
            method = argv[++i];
        }
        else if(!strcmp(argv[i], "-complete")){
            complete = argv[++i];
        }
        else{
            cout << "Unexpected parameter " << argv[i] << endl;
            validArgs = false;
        }
    }
    if (i != argc){
        cout << "Unexpected parameter " << argv[i] << endl;
        validArgs = false;
    }
    if(!validArgs)
        return false;

    
    if (input_file.empty()){
        std::cout << "Enter path of input file: ";
        std::cin >> input_file;
    }
    if (output_file.empty()){
        std::cout << "Enter path of output file: ";
        std::cin >> output_file;
    }
    if (config_file.empty()){
        std::cout << "Enter path of configuration file: ";
        std::cin >> config_file;
    }
    
    return true;
}
