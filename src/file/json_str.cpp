#ifndef JSON_STR_CPP
#define JSON_STR_CPP

#include "json_str.h"
#include <fmt/core.h>
#include <string>

void json_str::print_json_key_value(std::string key, std::string value)
{
  str += fmt::format("\"{}\":\"{}\",", key, value);
  return;
}

void json_str::print_json_class_head(std::string value)
{
  str += "\"";
  str += value;
  str += "\":{";
  return;
}

void json_str::print_json_class_end()
{
  str += "},";
  return;
}

void json_str::print_json_key(std::string key)
{
  str += "\"";
  str += key;
  str += "\":";
  return;
}

void json_str::print_json_value(std::string value)
{
  str += "\"";
  str += value;
  str += "\",";
  return;
}

std::string &json_str::get_str_ref()
{
  return str;
}
#endif