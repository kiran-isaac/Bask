//
// Created by kiran on 5/20/24.
//

#include <cstring>
#include <iostream>
#include <options.h>
#include <unistd.h>

using namespace std;

CommandLineArguments::CommandLineArguments(int argc, const char **argv) {
  cwd = getcwd(nullptr, 0);

  mode = IR;

  isStdin = false;

  argv++;
  argc--;

  // while (argc > 0) {
  //   if (argv[0][0] == '-') {
  //     if (strcmp(argv[0], "-o") == 0) {
  //       if (!out.empty()) {
  //         cerr << "Error: -o specified multiple times" << endl;
  //         exit(1);
  //       }

  //       if (argc < 2) {
  //         cerr << "Error: -o requires an argument" << endl;
  //         exit(1);
  //       }

  //       out = argv[1];

  //       argv++;
  //       argc--;
  //     } else {
  //       cerr << "Error: unknown option " << argv[0] << endl;
  //       exit(1);
  //     }
  //   } else {
  //     file = argv[0];

  //     // check if file exists
  //     if (access(file.c_str(), F_OK) == -1) {
  //       cerr << "Error: file " << file << " does not exist" << endl;
  //       cerr << "  Current working directory: " << cwd << endl;
  //       exit(1);
  //     }
  //   }
  //   argv++;
  //   argc--;
  // }

  unsigned i = 0;
  while (i < argc) {
    if (argv[i][0] == '-' && argv[i][1] != '\0') {
      switch (argv[i][1]) {
      case 'o':
        if (!out.empty()) {
          cerr << "Error: -o specified multiple times" << endl;
          exit(1);
        }

        if (argv[i][2] != '\0') {
          out = string(argv[i] + 2);
          i++;
          continue;
        }

        if (i + 1 >= argc) {
          cerr << "Error: -o requires an argument" << endl;
          exit(1);
        }

        out = argv[i + 1];

        i += 2;
        break;
      case 'c':
        mode = COMPILE;
        i++;
        break;
      case 'r':
        mode = RUN;
        i++;
        break;
      case 'i':
        mode = IR;
        i++;
        break;
      case 'a':
        mode = AST;
        i++;
        break;
      case 'h':
        mode = HELP;
        i++;
        break;
      }
    } else {
      file = argv[i];

      if (file == "-") {
        isStdin = true;
        i++;
        continue;
      }

      // check if file exists
      if (access(file.c_str(), F_OK) == -1) {
        cerr << "Error: file " << file << " does not exist" << endl;
        cerr << "  Current working directory: " << cwd << endl;
        exit(1);
      }
      i++;
    }
  }
}