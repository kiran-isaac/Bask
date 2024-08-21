//
// Created by kiran on 5/20/24.
//

#ifndef BASK_OPTIONS_H
#define BASK_OPTIONS_H

#include <string>
#include <vector>

using namespace std;

class CommandLineArguments {
public:
  string cwd;
  string file;
  string out;
  string lib_path;
  string opt_level;
  bool isStdin;
  enum Mode { COMPILE, RUN, IR, AST, HELP } mode;

  string find_module(string module_name);
  
  CommandLineArguments(int argc, const char **argv);

  CommandLineArguments(const CommandLineArguments &other) {
    cwd = other.cwd;
    file = other.file;
    out = other.out;
    lib_path = other.lib_path;
    isStdin = other.isStdin;
    mode = other.mode;
  }
};

#endif //BASK_OPTIONS_H
