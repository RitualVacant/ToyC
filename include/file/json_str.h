#ifndef JSON_STR_H
#define JSON_STR_H

#include <string>

class json_str
{
private:
  std::string str;

public:
  std::string &get_str_ref();

  void print_json_key_value(std::string key, std::string value);
  void print_json_class_head(std::string value);
  void print_json_class_end();
  void print_json_key(std::string key);
  void print_json_value(std::string value);
};

#endif