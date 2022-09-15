#ifndef SCANNING_CPP
#define SCANNING_CPP

#include "scanning.h"
#include "global_var.h"
#include "inner.h"
#include "judge_char.h"
#include "token.h"

namespace toy_c {

void scanning::get_next_char() {
  ++column;
  c = static_cast<char>(file.get());
}

std::tuple<token, std::string> scanning::to_number(bool is_negative) {
  std::string re;
  bool        is_double = false;
  re += c;
  get_next_char();
  while (is_number(c) || c == '.') {
    if (c == '.')
      is_double = true;
    re += c;
    get_next_char();
  }
  if (is_double) {
    if (is_negative) {
      now_token = std::make_tuple(token::constant_negative_float_number, re);
      return now_token;
    }
    else {
      now_token = std::make_tuple(token::constant_float_number, re);
      return now_token;
    }
  }
  else {
    if (is_negative) {
      now_token = std::make_tuple(token::constant_negative_integer_number, re);
      return now_token;
    }
    else {
      now_token = std::make_tuple(token::constant_integer_number, re);
      return now_token;
    }
  }
}

std::tuple<token, std::string> scanning::to_keyword_or_indentif() {
  std::string re;
  // bool is_key_word = false;
  re += c;
  get_next_char();
  while (is_number(c) || is_char(c) || c == '_') {
    re += c;
    get_next_char();
  }
  if (key_words.find(re) == key_words.end()) {
    now_token = std::make_tuple(token::identify, re);
    return std::make_tuple(token::identify, re);
  }
  else {
    now_token = std::make_tuple(key_words.at(re), re);
    return std::make_tuple(key_words.at(re), re);
  }
}

// inline std::tuple<token, std::string> scanning::to_string() {
//     std::string re;
//     get_next_char();
//     while (c != '"') {
//         re += c;
//         get_next_char();
//     }
//     get_next_char();    //pass string end table "
//     now_token = std::make_tuple(token::class_string, re);
//     return std::make_tuple(token::class_string, re);
// }

std::tuple<token, std::string> scanning::to_comment() {
  std::string re;
  get_next_char();
  while (c != '\n') {
    re += c;
    get_next_char();
  }
  now_token = std::make_tuple(token::comment, re);
  return std::make_tuple(token::comment, re);
}

std::tuple<token, std::string> scanning::to_char() {
  get_next_char();
  std::string re;
  re += c;
  get_next_char();
  now_token = std::make_tuple(token::r_char, re);
  return std::make_tuple(token::r_char, re);
}


token scanning::get_pre_token() {
  pre_token         = next_token();
  really_next_token = false;
  now_token         = last_token;
  return std::get<0>(pre_token);
}


std::tuple<token, std::string> scanning::next_token() {
  if (!really_next_token) {
    really_next_token = true;
    now_token         = pre_token;
    return pre_token;
  }
  last_token = now_token;
  std::string re;
  while (is_useless(c)) {
    if (c == '\n') {
      ++line;
      column = 0;
    }
    get_next_char();
  }
  switch (c) {
    case '?':
      get_next_char();
      now_token = std::make_tuple(token::question_mark, "?");
      return now_token;

    case '[':
      get_next_char();
      now_token = std::make_tuple(token::l_mid_par, "[");
      return std::make_tuple(token::l_mid_par, "[");

    case ']':
      get_next_char();
      now_token = std::make_tuple(token::r_mid_par, "]");
      return std::make_tuple(token::r_mid_par, "]");

    case '{':
      get_next_char();
      now_token = std::make_tuple(token::l_big_par, "{");
      return std::make_tuple(token::l_big_par, "{");

    case '}':
      get_next_char();
      now_token = std::make_tuple(token::r_big_par, "}");
      return std::make_tuple(token::r_big_par, "}");

    case '(':
      get_next_char();
      now_token = std::make_tuple(token::l_par, "(");
      return std::make_tuple(token::l_par, "(");

    case ')':
      get_next_char();
      now_token = std::make_tuple(token::r_par, ")");
      return std::make_tuple(token::r_par, ")");

    case ',':
      get_next_char();
      now_token = std::make_tuple(token::comma, ",");
      return std::make_tuple(token::comma, ",");

    case ';':
      get_next_char();
      now_token = std::make_tuple(token::end, ";");
      return std::make_tuple(token::end, ";");

    case ':':
      get_next_char();
      now_token = std::make_tuple(token::key_quotation, ":");
      return now_token;

    case '~':
      get_next_char();
      now_token = std::make_tuple(token::bit_not, "~");
      return std::make_tuple(token::bit_not, "~");

    case '+':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::plus_agn, "+=");
        return std::make_tuple(token::plus_agn, "+=");
      }
      else if (c == '+') {
        get_next_char();
        now_token = std::make_tuple(token::self_plus, "++");
        return now_token;
      }
      else {
        now_token = std::make_tuple(token::plus, "+");
        return std::make_tuple(token::plus, "+");
      }

    case '-':
      // -=
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::minus_agn, "-=");
        return std::make_tuple(token::minus_agn, "-=");
      }
      // --
      else if (c == '-') {
        get_next_char();
        now_token = std::make_tuple(token::self_minus, "--");
        return now_token;
      }
      // ->
      else if (c == '>') {
        get_next_char();
        now_token = std::make_tuple(token::ver, "->");
        return std::make_tuple(token::ver, "->");
      }
      // negative number
      else if (is_number(c)) {
        return to_number(true);
      }
      // signal -
      else {
        now_token = std::make_tuple(token::minus, "-");
        return std::make_tuple(token::minus, "-");
      }

    case '*':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::times_agn, "*=");
        return std::make_tuple(token::times_agn, "*=");
      }
      else {
        now_token = std::make_tuple(token::times, "*");
        return std::make_tuple(token::times, "*");
      }

    case '/':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::div_agn, "/=");
        return std::make_tuple(token::div_agn, "/=");
      }
      else {
        now_token = std::make_tuple(token::div, "/");
        return std::make_tuple(token::div, "/");
      }

    case '%':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::mod_agn, "%=");
        return std::make_tuple(token::mod_agn, "%=");
      }
      else {
        now_token = std::make_tuple(token::div, "%");
        return std::make_tuple(token::div, "%");
      }

    case '=':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::equ, "==");
        return std::make_tuple(token::equ, "==");
      }
      else {
        now_token = std::make_tuple(token::assign, "=");
        return std::make_tuple(token::assign, "=");
      }

    case '!':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::not_equ, "!=");
        return std::make_tuple(token::not_equ, "!=");
      }
      else {
        now_token = std::make_tuple(token::log_not, "!");
        return std::make_tuple(token::log_not, "!");
      }

    case '>':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::great_equ, ">=");
        return std::make_tuple(token::great_equ, ">=");
      }
      else if (c == '>') {
        get_next_char();
        if (c == '=') {
          get_next_char();
          now_token = std::make_tuple(token::r_shift_agn, ">>=");
          return now_token;
        }
        else {
          now_token = std::make_tuple(token::r_shift, ">>");
          return now_token;
        }
      }
      else {
        now_token = std::make_tuple(token::great, ">");
        return std::make_tuple(token::great, ">");
      }

    case '<':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::less_equ, "<=");
        return std::make_tuple(token::less_equ, "<=");
      }
      else if (c == '<') {
        get_next_char();
        if (c == '=') {
          get_next_char();
          now_token = std::make_tuple(token::l_shift_agn, "<<=");
          return now_token;
        }
        else {
          now_token = std::make_tuple(token::l_shift, "<<");
          return now_token;
        }
      }
      else {
        now_token = std::make_tuple(token::less, "<");
        return std::make_tuple(token::less, "<");
      }

    case '&':
      get_next_char();
      if (c == '&') {
        get_next_char();
        now_token = std::make_tuple(token::log_and, "&&");
        return std::make_tuple(token::log_and, "&&");
      }
      else if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::bit_and_agn, "&=");
        return now_token;
      }
      else {
        now_token = std::make_tuple(token::bit_and, "&");
        return std::make_tuple(token::bit_and, "&");
      }

    case '|':
      get_next_char();
      if (c == '|') {
        get_next_char();
        now_token = std::make_tuple(token::log_or, "||");
        return now_token;
      }
      else if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::bit_or_agn, "|=");
        return now_token;
      }
      else {
        now_token = std::make_tuple(token::bit_or, "|");
        return std::make_tuple(token::bit_or, "|");
      }

    case '^':
      get_next_char();
      if (c == '=') {
        get_next_char();
        now_token = std::make_tuple(token::bit_xor_agn, "^=");
        return now_token;
      }
      else {
        now_token = std::make_tuple(token::bit_xor, "^");
        return now_token;
      }


    // TODO
    // case after / eg: " '
    case '"': {
      get_next_char();
      std::string str;
      while (c != '"') {
        if (c == '/') {
          get_next_char();
          switch (c) {
            case '"':
              str += '"';
            case '\'':
              str += '\'';
            default:
              SWITCH_ERROR
          }
        }
        else {
          str += c;
          get_next_char();
        }
      }
      // eat last "
      get_next_char();
      return std::make_tuple(token::constant_string, str);
    }


    case '\'':
      return to_char();

    default:
      if (is_number(c))
        return to_number();
      if (is_char(c))
        return to_keyword_or_indentif();
      break;
  }
  get_next_char();
  return std::make_tuple(token::invalid, "");
}

//测试词法分析器函数
void scanning::token_output() {
  /*
  while (!file.eof()) {
    auto a = next_token();
    if (token_to_string.find(std::get<0>(a)) == token_to_string.end()) {
      fmt::print("[token:] identify   [string:] \"{}\"\n", std::get<1>(a));
    }
    else if (token_to_string.find(std::get<0>(a)) == token_to_string.end()
                    && key_words.find(std::get<1>(a)) != key_words.end()
                )
                {
      fmt::print("can't print token cause it is not involved in token_to_string");
      exit(0);
    }
    else {
      fmt::print(
        "[token:] {:10} [string:] \"{}\"\n", token_to_string.at(std::get<0>(a)),
        std::get<1>(a)
      );
    }
  }
  */

  toy_c::fstream_guard output_file(output_file_path, toy_c::mode::write);
  while (!file.eof()) {
    auto a = next_token();
    if (std::get<0>(a) == token::identify) {
      output_file << fmt::format("token: {:30} string: {}\n", "identify", std::get<1>(a));
    }
    else {
      output_file << fmt::format(
        "token: {:30} string: {}\n", token_to_string.at(std::get<0>(a)), std::get<1>(a)
      );
    }
  }
}

token scanning::get_current_token(std::tuple<token, std::string> &tuple_) {
  return std::get<0>(tuple_);
}

token scanning::get_current_token() {
  return std::get<0>(now_token);
}

std::string scanning::get_current_value(std::tuple<token, std::string> &tuple_) {
  return std::get<1>(tuple_);
}

std::string scanning::get_current_value() {
  return std::get<1>(now_token);
}

std::size_t scanning::get_line() {
  return line;
}
std::size_t scanning::get_column() {
  return column;
}

}  // namespace toy_c
#endif