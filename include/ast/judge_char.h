#ifndef JUDGE_CHAR_H
#define JUDGE_CHAR_H

#pragma once
#include "token.h"
#include <string>
#include <unordered_map>

extern char                                         useless_char[];
extern const std::unordered_map<token, std::string> token_to_string;
extern std::unordered_map<std::string, token>       key_words;

extern bool is_useless(char const c);
extern bool is_number(char const c);
extern bool is_char(char const c);
extern bool is_binary_operator(token operator_);
extern bool is_primary_operator(token token_);
extern bool is_basic_type(token token_);
// DROP
extern short unsigned int preceden(token operator_);

#endif