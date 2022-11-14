#ifndef SPEC_SYMBOL_TABLE_CPP
#define SPEC_SYMBOL_TABLE_CPP

#include "spec_symbol_table.h"
#include <fmt/color.h>
#include <fmt/core.h>

namespace spt
{

void spec_symbol_table::push_scope()
{
  table.push_back({});
}

void spec_symbol_table::pop_scope()
{
  table.pop_back();
}

void spec_symbol_table::insert_symbol(std::string &name, Type *type)
{
  if (table.size() == 0)
  {
    fmt::print(fg(fmt::color::red), "symbol table don't have scope\n", name);
    exit(0);
  }
  table.back().push_back({name, type});
}

Type *spec_symbol_table::get_type(std::string &name)
{
  for (auto &i : table)
  {
    for (auto &j : i)
    {
      if (j.name == name)
      {
        return j.type;
      }
    }
  }

  fmt::print(fg(fmt::color::red), "can't find define or declare of {}\n", name);
  for (auto &i : table)
  {
    fmt::print("-------------------\n");
    for (auto &j : i)
    {
      fmt::print("identifier: {}\n", j.name);
    }
  }
  fmt::print("-------------------\n");
  exit(0);
}

}  // namespace spt

#endif