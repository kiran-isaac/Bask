//
// Created by kiran on 5/20/24.
//

#include <options.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

Options::Options(int argc, const char **argv) {
  cwd = getcwd(nullptr, 0);
  
  argv++;
  argc--;
  
  while (argc > 0) {
    if (argv[0][0] == '-') {
      if (strcmp(argv[0], "-o") == 0) {
        if (!out.empty()) {
          cerr << "Error: -o specified multiple times" << endl;
          exit(1);
        }
        
        if (argc < 2) {
          cerr << "Error: -o requires an argument" << endl;
          exit(1);
        }
        
        out = argv[1];
        
        argv++;
        argc--;
      } else {
        cerr << "Error: unknown option " << argv[0] << endl;
        exit(1);
      }
    } else {
      file = argv[0];
      
      // check if file exists
      if (access(file.c_str(), F_OK) == -1) {
        cerr << "Error: file " << file << " does not exist" << endl;
        cerr << "  Current working directory: " << cwd << endl;
        exit(1);
      }
    }
    argv++;
    argc--;
  }
}