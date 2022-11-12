#ifndef JSON_STR_H
#define JSON_STR_H

#include <string>

class json_str
{
private:
  std::string str;
  // int64_t     class_num;

public:
  void init();
  void end();

  std::string &get_str_ref();

  void print_key_value(std::string key, std::string value);
  void print_json_class(std::string value, auto func)
  {
    str += "\"";
    str += value;
    str += "\":{";
    func();
    str += "},";
    // ++class_num;
  }
  void print_json_array(std::string value, auto func)
  {
    str += "\"";
    str += value;
    str += "\":[";
    func();
    str += "],";
  }
};

#endif