#ifndef JUDGE_CHAR_H
#define JUDGE_CHAR_H

#pragma once
#include <string>
#include "token.h"
#include <unordered_map>


char const useless_char[] = {
    '\n',
    '\r',
    '\t',
    ' '
};

//测试词法划分是否正确的输出转换
std::unordered_map<token, std::string> const token_to_string {
    {token::invalid, "invalid"},
    {token::jmp, "jmp"},
    {token::lable, "lable"},

    {token::dec_int,         "dec_int"},
    {token::dec_double,      "dec_double"},
    {token::dec_char,        "dec_char"},
    {token::dec_bool,        "dec_bool"},

    {token::dec_int_arrary,     "dec_int_arrary"},
    {token::dec_double_arrary,  "dec_double_arrary"},
    {token::dec_char_arrary,    "dec_char_arrary"},
    {token::dec_bool_arrary,    "dec_bool_arrary"},

    {token::dec_func,           "dec_func"},
    {token::call_func,          "call_func"},
    {token::call_func_end,      "call_func_end"},
    {token::func_return,        "func_return"},

    {token::arg_r_int,          "arg_r_int"},
    {token::arg_r_double,       "arg_r_double"},
    {token::arg_r_char,         "arg_r_char"},
    {token::arg_r_bool,         "arg_r_bool"},

    {token::arg_int_class,      "arg_int_class"},
    {token::arg_double_class,   "arg_double_class"},
    {token::arg_char_class,     "arg_char_class"},
    {token::arg_bool_class,     "arg_bool_class"},

    {token::class_int,       "class_int"},
    {token::class_double,    "class_double"},
    {token::class_char,      "class_char"},
    {token::class_bool,      "class_bool"},

    {token::class_int_array,    "class_int_array"},
    {token::class_double_array, "class_double_array"},
    {token::class_char_array,   "class_char_array"},

    {token::key_void,           "key_void"},
    {token::key_int,            "key_int"},
    {token::key_long,           "key_long"},
    {token::key_double,         "key_double"},
    {token::key_char,           "key_char"},
    {token::key_unsigned,       "key_unsigned"},
    {token::key_do,             "key_do"},
    {token::key_default,        "default"},

    {token::r_int,          "r_int"},
    {token::r_double,       "r_double"},
    {token::r_char,         "r_char"},
    {token::r_bool,         "r_bool"},
    {token::bit_and,        "bit_and"},
    {token::bit_or,         "bit_or"},
    {token::bit_not,        "bit_not"},
    {token::log_and,        "log_and"},
    {token::log_or,         "log_or"},
    {token::log_not,        "log_not"},
    {token::plus,           "plus"},
    {token::minus,          "minus"},
    {token::times,          "times"},
    {token::div,            "div"},
    {token::mod,            "mod"},
    {token::ver,            "ver"},
    {token::equ,            "equ"},
    {token::not_equ,        "not_equ"},
    {token::great_equ,      "great_equ"},
    {token::less_equ,       "less_equ"},
    {token::great,          "great"},
    {token::less,           "less"},
    {token::assign,         "assign"},
    {token::plus_agn,       "plus_agn"},
    {token::minus_agn,      "minus_agn"},
    {token::times_agn,      "times_agn"},
    {token::div_agn,        "div_agn"},
    {token::mod_agn,        "mod_agn"},
    {token::l_par,          "l_par"},
    {token::r_par,          "r_par"},
    {token::l_mid_par,      "l_mid_par"},
    {token::r_mid_par,      "r_mid_par"},
    {token::l_big_par,      "l_big_par"},
    {token::r_big_par,      "r_big_par"},
    {token::comma,         "comma"},
    {token::comment,       "comment"},
    {token::end,           "end"},
    {token::identif,         "identif"},
    {token::key_if,           "key_if"},
    {token::key_else,         "key_else"},
    {token::key_true,         "key_true"},
    {token::key_false,        "key_false"},
    {token::key_while,        "key_while"},
    {token::key_for,          "key_for"},
    {token::key_null,         "key_null"},
    {token::key_func,         "key_func"},
    {token::key_return,       "key_return"},
    {token::key_break,        "key_break"},
    {token::key_continue,     "key_continue"},
};

//find key words
std::unordered_map<std::string, token> const key_words {
    {"if",       token::key_if},
    {"else",     token::key_else},
    {"true",     token::key_true},
    {"false",    token::key_false},
    {"while",    token::key_while},
    {"for",      token::key_for},
    {"null",     token::key_null},
    {"return",   token::key_return},
    {"break",    token::key_break},
    {"continue", token::key_continue},

    {"struct",   token::key_struct},

    {"int",      token::key_int},
    {"double",   token::key_double},
    {"char",     token::key_char},
    {"long",     token::key_long},
    {"void",     token::key_void},
    {"extern",   token::key_extern},

    {"switch",   token::key_switch},
    {"do",       token::key_do},
    {"for",      token::key_for},
    {"case",     token::key_case},
    {"default",  token::key_default},

    {"auto",     token::key_auto},
    {"break",    token::key_break},
    {"case",     token::key_case},
    {"const",    token::key_const},
    {"enum",     token::key_enum},
    {"inline",   token::key_inline},
    {"register", token::key_register},
    {"restrict", token::key_restrict},
    {"signed",   token::key_signed},
    {"short",    token::key_short},
    {"sizeof",   token::key_sizeof},
    {"static",   token::key_static},
    {"switch",   token::key_switch},
    {"typedef",  token::key_typedef},
    {"union",    token::key_union},
    {"void",     token::key_void},
    {"volatile", token::key_volatile},
    {"_Bool",    token::key__Bool},
    {"Complex",  token::key__Complex},
    {"_Imaginary",token::key__Imaginary},
};

inline bool is_useless(char const c) {
    for (char i : useless_char) {
        if (i == c) return true;
    }
    return false;
}
inline bool is_number(char const c) {
    if (c >= '0' && c <= '9') return true;
    else return false;
}
inline bool is_char(char const c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') return true;
    else return false;
}
//二元运算符
inline bool is_binary_operator(token operator_) {
    if (
        operator_ == token::bit_and ||
        operator_ == token::bit_not ||
        operator_ == token::bit_or ||
        operator_ == token::log_and ||
        operator_ == token::log_not ||
        operator_ == token::log_or ||
        operator_ == token::equ ||
        operator_ == token::not_equ ||
        operator_ == token::great ||
        operator_ == token::great_equ ||
        operator_ == token::less ||
        operator_ == token::less_equ ||
        operator_ == token::plus ||
        operator_ == token::minus ||
        operator_ == token::times ||
        operator_ == token::div ||
        operator_ == token::mod
    ) return true;
    else return false;
}
//公共运算符
inline bool is_primary_operator(token token_) {
    if (
        token_ == token::l_par ||
        token_ == token::r_par ||
        token_ == token::l_mid_par ||
        token_ == token::r_mid_par ||
        token_ == token::class_int ||
        token_ == token::class_char ||
        token_ == token::class_double ||
        token_ == token::class_bool
    ) return true;
    return false;
}
inline bool is_basic_type(token token_) {
    switch (token_) {
        case token::class_int:
            return true;
        case token::class_bool:
            return true;
        case token::class_double:
            return true;
        case token::class_char:
            return true;
        default:
            return false;
    }
    return false;
}
short unsigned int preceden(token operator_) {
    switch (operator_) {
    case token::log_not:
        return 1;

    case token::log_and:
        return 2;

    case token::equ:
    case token::not_equ:
    case token::great:
    case token::great_equ:
    case token::less:
    case token::less_equ:
        return 3;

    case token::plus:
    case token::minus:
    case token::bit_or:
        return 4;

    case token::times:
    case token::mod:
    case token::div:
    case token::bit_and:
        return 5;

    default:
        return 0;
    }
}

#endif