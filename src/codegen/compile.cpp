#include "options.h"
#include <codegen.h>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <options.h>
#include <unistd.h>

#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>

void BASKCodeGenVisitor::compileModule(CommandLineArguments &options) {
  string IR = getModuleAsString();

  // make temp dir if it doesn't exist
  system("mkdir -p /tmp/bask");

  // temp file
  string temp_file = "/tmp/bask/ir.ll";
  ofstream out(temp_file);
  out << IR;

  out.close();

  // Check if lib path exists
  if (access((options.lib_path + "/lib.bc").c_str(), F_OK) == -1) {
    cerr << "Error: expected to find stdlib at " << options.lib_path << "/lib.bc" << " but could not :(" << endl;
    exit(1);
  }

  // compile
  string cmd = "clang " + temp_file + " " + options.lib_path + "/lib.bc" + " -o " + (options.out.empty() ? "a.out" : options.out) + " 2> /tmp/bask/clang.log";

  if (system(cmd.c_str()) != 0) {
    cerr << "Error: failed to compile" << endl;
    cerr << "Clang invocation: " << cmd << endl;

    ifstream log("/tmp/bask/clang.log");
    string line;
    while (getline(log, line)) {
      cerr << line << endl;
    }

    cerr << "Clang log dumped to /tmp/bask/clang.log" << endl;

    string ir = getModuleAsString();
    ofstream ir_out("/tmp/bask/ir.ll");
    ir_out << ir;
    ir_out.close();

    cerr << "IR dumped to /tmp/bask/ir.ll" << endl;
    exit(1);
  } else {
    cout << "Compiled successfully" << endl;
    system("rm -rf /tmp/bask");
    exit(0);
  }
}