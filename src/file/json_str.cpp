#ifndef JSON_STR_CPP
#define JSON_STR_CPP

#include "json_str.h"
#include <fmt/core.h>
#include <string>

void json_str::init()
{
  // class_num = 0;
  str.clear();
  str += "@startjson\n{";
}

void json_str::end()
{
  str += "}\n@endjson";
  for (std::size_t i = 0, j = 1; j < str.size(); ++i, ++j)
  {
    if (str[i] == ',' && (str[j] == '}' || str[j] == ']'))
    {
      str[i] = ' ';
    }
  }
}

void json_str::print_key_value(std::string key, std::string value)
{
  str += fmt::format("\"{}\":\"{}\",", key, value);
}

std::string &json_str::get_str_ref()
{
  return str;
}

#endif