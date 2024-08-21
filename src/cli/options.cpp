//
// Created by kiran on 5/20/24.
//

#include <cstddef>
#include <cstring>
#include <iostream>
#include <options.h>
#include <unistd.h>

using namespace std;

string CommandLineArguments::find_module(string module_name) {
  // get cwd 
  string cwd = getcwd(nullptr, 0);

  string lib_path = this->lib_path;

  // check if module_name is an absolute path
  if (module_name[0] == '/') {
    if (access(module_name.c_str(), F_OK) == -1) {
      cerr << "Error: file " << module_name << " does not exist" << endl;
      cerr << "  Current working directory: " << cwd << endl;
      exit(1);
    }

    return module_name;
  }

  // check if module_name is a relative path
  if (access((cwd + "/" + module_name).c_str(), F_OK) != -1) {
    return cwd + "/" + module_name;
  }

  // check if module_name is in lib_path
  if (!lib_path.empty()) {
    if (access((lib_path + "/" + module_name).c_str(), F_OK) != -1) {
      return lib_path + "/" + module_name;
    }
  }

  return "";
}

CommandLineArguments::CommandLineArguments(int argc, const char **argv) {
  cwd = getcwd(nullptr, 0);

  mode = COMPILE;

  isStdin = false;

  argv++;
  argc--;

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
      case 'l':
        if (!lib_path.empty()) {
          cerr << "Error: -l specified multiple times" << endl;
          exit(1);
        }

        if (argv[i][2] != '\0') {
          lib_path = string(argv[i] + 2);
          i++;
          continue;
        } else {
          if (i + 1 >= argc) {
            cerr << "Error: -l requires an argument" << endl;
            exit(1);
          }

          lib_path = argv[i + 1];
          i += 2;
        }
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

  // if in help mode or no args given, display help message 
  if (mode == HELP || argc == 0) {
    cout << "Usage: bask [options] [file]" << endl;
    cout << "Options:" << endl;
    cout << "  -o <file>  Output file" << endl;
    cout << "  -l <path>  Library path" << endl;
    cout << "  -c         Compile" << endl;
    cout << "  -r         Run" << endl;
    cout << "  -i         Generate IR" << endl;
    cout << "  -a         Generate AST" << endl;
    cout << "  -h         Display this help message" << endl;
    exit(0);
  }

  if (lib_path.empty()) {
    lib_path = "/usr/local/lib/bask";
  }

  if (out.empty()) {
    switch(mode) {
      case IR:
        break;
      case COMPILE:
        out = "a.out";
        break;
      case RUN:
        out = "a.out";
        break;
      case AST:
        break;
      case HELP:
        break;
    }
  }
}