//
// Created by lzj on 2023.6.20.
//

#ifndef TOYC_PARSER_ERROR_H
#define TOYC_PARSER_ERROR_H

#include "token/Token.h"
#include "token/TokenType.h"

namespace toy_c::parser
{

extern void error_unexpect_token(token::Token unexpect_token_);

extern void error_unexpect_token(
  token::Token const                     &unexpect_token_,
  std::initializer_list<token::TokenType> expect_token_list_
);

#define ERROR_UNEXPECT_TOKEN_1arg(unexpect_token_)                                       \
  fmt::print(fg(fmt::color::yellow_green), "at {}:{}\n", __FILE__, __LINE__);            \
  error_unexpect_token(unexpect_token_)

#define ERROR_UNEXPECT_TOKEN_2arg(unexpect_token_, expect_token_list_)                   \
  fmt::print(fg(fmt::color::yellow_green), "at {}:{}\n", __FILE__, __LINE__);            \
  error_unexpect_token(unexpect_token_, expect_token_list_);

}  // namespace toy_c::parser


#endif  // TOYC_PARSER_ERROR_H
