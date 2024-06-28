//
// Created by kiran on 5/21/24.
//

#ifndef KL_SYMTAB_H
#define KL_SYMTAB_H

#include <string>
#include <types.h>
#include <unordered_map>
#include <optional>
#include <stack>
#include <iostream>

class SymTab {
private:
  std::vector<std::unordered_map<std::string, KL_Type>> scope_stack;

public:
  SymTab() {
    scope_stack.emplace_back();
  }
  bool name_is_in_scope(std::string);
  bool name_is_in_scope(const char *name) {
    return name_is_in_scope(std::string(name));
  }
  std::optional<KL_Type> get_name_type(std::string);
  void add_name(std::string, KL_Type);
  void enter_block();
  void exit_block();
  void wipe() {
    scope_stack.clear();
    enter_block();
  }
  void print() {
    cout << "Scope Stack" << endl;
    for (auto scope = scope_stack.rbegin(); scope != scope_stack.rend(); scope++) {
      for (auto &pair : *scope) {
        std::cout << pair.first << " : " << pair.second.to_string() << ", ";
      }
      std::cout << std::endl;
    }

  }

  unsigned int get_scope_depth() {
    return scope_stack.size();
  }
};

#endif //KL_SYMTAB_H
