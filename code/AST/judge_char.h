#ifndef HEADER
#define HEADER
#include "./../header.h"
#endif

#ifndef TOKEN
#define TOKEN
#include "./token.h"
#endif

#ifndef UNORDERED_MAP
#define UNORDERED_MAP
#include <unordered_map>
#endif

char const useless_char[] = {
    '\n',
    '\r',
    '\t',
    ' '
};

//测试词法划分是否正确的输出转换
std::unordered_map<token, std::string> const test_output_token {
    {token::invalid, "0"},

    {token::class_int,     "int"},
    {token::class_double,  "double"},
    {token::class_string,  "string"},
    {token::class_char,    "char"},

    {token::bit_and, "&"},
    {token::bit_or,  "|"},
    {token::bit_not, "~"},

    {token::log_and, "&&"},
    {token::log_or,  "||"},
    {token::log_not, "~"},

    {token::plus,  "+"},
    {token::minus, "-"},
    {token::times, "*"},
    {token::div,   "/"},
    {token::mod,   "%"},

    {token::equ,        "=="},
    {token::not_equ,    "!="},
    {token::great_equ,  ">="},
    {token::less_equ,   "<="},
    {token::great,      ">"},
    {token::less,       "<"},


    {token::assign,     "="},
    {token::plus_agn,   "+="},
    {token::minus_agn,  "-="},
    {token::times_agn,  "*="},
    {token::div_agn,    "/="},
    {token::mod_agn,    "%="},

    {token::l_par,       "("},
    {token::r_par,       ")"},
    {token::l_mid_par,   "["},
    {token::r_mid_par,   "]"},
    {token::l_big_par,   "{"},
    {token::r_big_par,   "}"},

    {token::comma,   ","},
    {token::comment, "#"},

    //{token::indentif,  "标识符"},
    {token::key_if,         "if"},
    {token::key_else,       "else"},
    {token::key_true,       "true"},
    {token::key_false,      "false"},
    {token::key_while,      "while"},
    {token::key_for,        "for"},
    {token::key_null,       "null"},
    {token::key_func,       "func"},
    {token::key_return,     "return"},
    {token::key_break,      "break"},
    {token::key_continue,   "continue"}
};

//查找关键字
std::unordered_map<std::string, token> const key_words {
    {"if",       token::key_if},
    {"else",     token::key_else},
    {"true",     token::key_true},
    {"false",    token::key_false},
    {"while",    token::key_while},
    {"for",      token::key_for},
    {"null",     token::key_null},
    {"func",     token::key_func},
    {"return",   token::key_return},
    {"break",    token::key_break},
    {"continue", token::key_continue}
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
        token_ == token::class_string ||
        token_ == token::class_double ||
        token_ == token::class_bool
    ) return true;
    return false;
}
short unsigned int precedence(token operator_) {
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