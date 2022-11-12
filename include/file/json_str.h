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

  void print_json_key_value(std::string key, std::string value);
  void print_json_class_head(std::string value);
  void print_json_class_end();
  void print_json_key(std::string key);
  void print_json_value(std::string value);
  void print_json_class(std::string value, auto func)
  {
    str += "\"";
    str += value;
    // str += '_';
    // str += std::to_string(class_num);
    str += "\":{";
    func();
    str += "},";
    // ++class_num;
  }
  void print_json_array(std::string value, auto func)
  {
    str += "\"";
    str += value;
    // str += '_';
    // str += std::to_string(class_num);
    str += "\":[";
    func();
    str += "],";
    // ++class_num;
  }
};

#endif