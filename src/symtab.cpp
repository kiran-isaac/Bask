//
// Created by kiran on 5/21/24.
//

#include <symtab.h>

optional<SymTabEntry> SymTab::get(const std::string &name) {
  if (entries.find(name) == entries.end()) {
    return nullopt;
  }
  
  SymTabEntry entry;
  entry.name = name;
  entry.type = entries[name].type;
  entry.scope = entries[name].scope;
  entry.stmt_num = entries[name].stmt_num;
  return entry;
}

bool SymTab::insert(const SymTabEntry &entry) {
  if (entries.find(entry.name) != entries.end()) {
    return false;
  }
  
  InternalEntry internal_entry;
  internal_entry.type = entry.type;
  internal_entry.scope = entry.scope;
  internal_entry.stmt_num = entry.stmt_num;
  entries[entry.name] = internal_entry;
  
  return true;
}