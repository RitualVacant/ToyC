//
// Created by lzj on 2023.6.20.
//

#include "token/Token.h"
#include "token/TokenType.h"
#include <fmt/color.h>
#include <fmt/core.h>


namespace toy_c::parser
{

/**
 *
 * @param unexpect_token_
 */
void error_unexpect_token(token::Token const unexpect_token_)
{
  fmt::print(
    fg(fmt::color::yellow_green), "unexpected token: {} \"{}\" at {}:{}\n",
    token::c_TokenType_map_string.at(unexpect_token_.type), unexpect_token_.str,
    unexpect_token_.line, unexpect_token_.column
  );

  exit(1);
}

/**
 *
 * @param unexpect_token_
 * @param expect_token_list_
 */
void error_unexpect_token(
  token::Token const                     &unexpect_token_,
  std::initializer_list<token::TokenType> expect_token_list_
)
{
  fmt::print(fg(fmt::color::yellow_green), "[ERROR]\n");
  fmt::print(fg(fmt::color::yellow_green), "expected token type:\n");
  for (auto &i : expect_token_list_)
  {
    fmt::print(
      fg(fmt::color::yellow_green), "-- {}\n", token::c_TokenType_map_string.at(i)
    );
  }

  fmt::print(
    fg(fmt::color::yellow_green), "but: {} \"{}\"\n",
    token::c_TokenType_map_string.at(unexpect_token_.type), unexpect_token_.str
  );
  fmt::print(
    fg(fmt::color::yellow_green), "at {}:{}\n", unexpect_token_.line,
    unexpect_token_.column
  );

  exit(1);
}

}  // namespace toy_c::parser
