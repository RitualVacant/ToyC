//
// Created by lzj on 2023.6.4.
//

#ifndef TOYC_TOKEN_TYPE_H
#define TOYC_TOKEN_TYPE_H

#include <string>
#include <unordered_map>

namespace toy_c::token
{

enum class TokenType : char
{
  invalid = 0,

  colon,           // :

  bit_and,         // &
  bit_or,          // |
  bit_not,         // ~
  bit_xor,         // ^

  log_and,         // &&
  log_or,          // ||
  log_not,         // !

  plus,            // +
  minus,           // -
  times,           // *
  div,             // /
  mod,             // %
  dot,             // .

  self_plus,       // ++
  self_minus,      // --

  arrow,           // ->
  equ,             // ==
  not_equ,         // !=
  great_equ,       // >=
  less_equ,        // <=
  great,           // >
  less,            // <
  l_shift,         // <<
  r_shift,         // >>

  assign,          // =
  plus_agn,        // +=
  minus_agn,       // -=
  times_agn,       // *=
  div_agn,         // /=
  mod_agn,         // %=
  r_shift_agn,     // >>=
  l_shift_agn,     // <<=
  bit_and_agn,     // &=
  bit_or_agn,      // |=
  bit_xor_agn,     // ^=

  question_mark,   // ?

  l_par,           // (
  r_par,           // )
  l_mid_par,       // [
  r_mid_par,       // ]
  l_big_par,       // {
  r_big_par,       // }

  comma,           // ,
  semicolon,       // ;

  identify,        // identify
  key_auto,        // auto
  key_const,       // const
  key_enum,        // enum
  key_inline,      // inline
  key_register,    // register
  key_restrict,    // restrict
  key_signed,      // signed
  key_short,       // short
  key_sizeof,      // sizeof
  key_static,      // static
  key_typedef,     // typedef
  key_union,       // union
  key_void,        // void
  key_volatile,    // volatile
  key__Bool,       // __Bool
  key__Complex,    // __Complex
  key__Imaginary,  // __Imaginary

  key_extern,      // extern

  key_int,         // int
  key_long,        // long
  key_double,      // double
  key_float,       // float
  key_unsigned,    // unsigned
  key_char,        // char
  key_struct,      // struct

  key_if,          // if
  key_else,        // else
  key_true,        // true
  key_false,       // false
  key_while,       // while
  key_goto,        // goto
  key_for,         // for
  key_null,        // null
  key_func,        // func
  key_return,      // return
  key_break,       // break
  key_continue,    // continue
  key_switch,      // switch
  key_do,          // do
  key_case,        // case
  key_default,     // default


  // literal value
  literal_string,
  literal_char,
  literal_positive_integer,
  literal_negative_integer,
  literal_positive_float,
  literal_negative_float,

  eof,
};

// keywords map TokenType
extern std::unordered_map<std::string, TokenType> const c_keywords_map_TokenType;
// TokenType map string
extern std::unordered_map<TokenType, std::string> const c_TokenType_map_string;


}  // namespace toy_c::token


#endif  // TOYC_TOKEN_TYPE_H
