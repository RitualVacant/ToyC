#ifndef WORNG_CPP
#define WORNG_CPP

#include "fmt/core.h"
#include "token.h"
#include <unordered_map>

/*
namespace worng {
    std::unordered_map<token, std::string> const trans_token_to_string {
        {token::invalid, "0"},

        {token::class_int,     "int"},
        {token::class_double,  "double"},
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

        {token::ver,        "->"},
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

        //{token::identif,  "标识符"},
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
        {token::key_continue,   "continue"},
    };
    //this funcion's argc will rewirte after
    //need really line and colume
    void redefine(std::size_t line, std::size_t colume, std::tuple<token, std::string> x) {
        fmt::print(fg(fmt::color::red), "a worng at line : {} , colume : {}\n", line, colume);
        fmt::print(fg(fmt::color::red), "you define {} twice!\n", std::get<1>(x));
        exit(0);
        return;
    }

    void lack_token(std::size_t line, std::size_t colume, std::tuple<token, std::string> x) {
        fmt::print(fg(fmt::color::red), "a worng at line : {} , colume : {}\n", line, colume);
        fmt::print(fg(fmt::color::red), "there should is a {}\n", trans_token_to_string.at(std::get<0>(x)));
        exit(0);
        return;
    }

    void havent_define(std::size_t line, std::size_t colume, std::tuple<token, std::string> x) {
        fmt::print(fg(fmt::color::red), "a worng at line : {} , colume : {}\n", line, colume);
        fmt::print(fg(fmt::color::red), "haven't defind \'{}\' {}\n", trans_token_to_string.at(std::get<0>(x)), std::get<1>(x));
        exit(0);
        return;
    }

    void rewirte(std::size_t line, std::size_t column, std::tuple<token, std::string> x) {
        return;
    }
}
*/

#endif