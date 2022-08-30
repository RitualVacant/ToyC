#ifndef SCANNING_H
#define SCANNING_H

#pragma once
#include "ast.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "fstream_guard.h"
#include "judge_char.h"
#include "token.h"
#include <fstream>
#include <stdlib.h>
#include <string>
#include <tuple>

namespace toy_c {

// line in the source code    loc: script/code/header/class/scanning.h
static unsigned int line = 1;
// column in the source code  loc: script/code/header/class/scanning.h
static unsigned int column = 1;

class scanning {
private:
  char        c;
  std::size_t num_token = 0;  // the number of token in this script file
  std::string file_path;
  bool        really_next_token = true;

  void get_next_char();

  std::tuple<token, std::string> to_number(bool is_negative = false);
  std::tuple<token, std::string> to_keyword_or_indentif();
  std::tuple<token, std::string> to_comment();
  std::tuple<token, std::string> to_char();
  // std::tuple<token, std::string> to_string();

public:
  toy_c::fstream_guard           file;
  std::tuple<token, std::string> now_token;
  std::tuple<token, std::string> last_token;
  std::tuple<token, std::string> pre_token;

  explicit scanning(std::string &file_path_) : file_path(file_path_) {
    file.open(file_path, toy_c::mode::read);
    c = file.get();
  };
  ~scanning(){};

  scanning(scanning const &)       = delete;
  scanning(scanning &&)            = default;
  scanning &operator=(scanning &)  = delete;
  scanning &operator=(scanning &&) = delete;

  std::tuple<token, std::string> next_token();
  token                          get_pre_token();
  token                          get_current_token();
  token       get_current_token(std::tuple<token, std::string> &tuple_);
  std::string get_current_value();
  std::string get_current_value(std::tuple<token, std::string> &tuple_);
  void        token_output();
};

}  // namespace toy_c
#endif