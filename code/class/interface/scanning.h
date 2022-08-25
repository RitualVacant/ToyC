#ifndef SCANNING_H
#define SCANNING_H

#pragma once
#include "ast.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "fstream_guard.cpp"
#include "judge_char.h"
#include "token.h"
#include <fstream>
#include <stdlib.h>
#include <string>
#include <tuple>

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

  std::tuple<token, std::string> to_number();
  std::tuple<token, std::string> to_keyword_or_indentif();
  std::tuple<token, std::string> to_comment();
  std::tuple<token, std::string> to_char();
  // std::tuple<token, std::string> to_string();

public:
  my::fstream_guard              file;
  std::tuple<token, std::string> now_token;
  std::tuple<token, std::string> last_token;
  std::tuple<token, std::string> pre_token;

  explicit scanning(std::string& file_path_) : file_path(file_path_) {
    file.open(file_path, my::mode::read);
    c = file.get();
  };
  ~scanning(){};

  scanning(scanning const&)       = delete;
  scanning(scanning&&)            = default;
  scanning& operator=(scanning&)  = delete;
  scanning& operator=(scanning&&) = delete;

  std::tuple<token, std::string> next_token();
  token                          get_pre_token();
  token                          get_current_token();
  token       get_current_token(std::tuple<token, std::string>& tuple_);
  std::string get_current_value();
  std::string get_current_value(std::tuple<token, std::string>& tuple_);

  //测试词法分析器函数
  void token_output() {
    while (!file.eof()) {
      auto a = next_token();
      if (token_to_string.find(std::get<0>(a)) == token_to_string.end()) {
        fmt::print("[token:] identif   [string:] \"{}\"\n", std::get<1>(a));
      }
      else if (token_to_string.find(std::get<0>(a)) == token_to_string.end()
                    && key_words.find(std::get<1>(a)) != key_words.end()
                )
                {
        fmt::print(
          "can't print token cause it is not involed in token_to_string"
        );
        exit(0);
      }
      else {
        fmt::print(
          "[token:] {:10} [string:] \"{}\"\n",
          token_to_string.at(std::get<0>(a)), std::get<1>(a)
        );
      }
    }
  }
};

#endif