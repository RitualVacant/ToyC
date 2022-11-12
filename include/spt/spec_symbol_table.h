#ifndef SPEC_SYMBOL_TABLE_H
#define SPEC_SYMBOL_TABLE_H

#include "spec_tree_node.h"
#include <string>
#include <vector>

namespace spt
{

class spec_symbol_table
{
private:
  struct table_item
  {
    std::string name;
    Type       *type;
  };
  std::vector<std::vector<table_item>> table;

public:
  void  push_scope();
  void  pop_scope();
  void  insert_symbol(std::string &name, Type *type);
  Type *get_type(std::string &name);
};

}  // namespace spt

#endif