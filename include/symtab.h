//
// Created by kiran on 5/21/24.
//

#ifndef KL_SYMTAB_H
#define KL_SYMTAB_H

#include <string>
#include <types.h>
#include <unordered_map>
#include <optional>

struct SymTabEntry {
  string name;
  
  KL_Type type;
  
  // Scope is the name of the function
  std::string scope;
  
  // number of statements from the beginning of the function to the declaration
  unsigned int stmt_num;
};

class SymTab {
private:
  SymTab() = default;
  
  struct InternalEntry {
    KL_Type type;
    string scope;
    unsigned int stmt_num;
  };
  
  std::unordered_map<std::string, InternalEntry> entries = {};
public:
  SymTab(const SymTab&) = delete;
  SymTab& operator=(const SymTab&) = delete;
  
  // Provide a static method to get the instance
  static SymTab& getInstance() {
    static SymTab instance;
    return instance;
  }
  
  bool insert(const SymTabEntry &entry);
  optional<SymTabEntry> get(const std::string &name);
};

#define SYMTAB SymTab::getInstance()

#endif //KL_SYMTAB_H
