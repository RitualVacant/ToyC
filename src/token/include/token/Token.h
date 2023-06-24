//
// Created by lzj on 2023.6.4.
//

#ifndef TOYC_TOKEN_H
#define TOYC_TOKEN_H

#include "token/TokenType.h"
#include <string>

namespace toy_c::token
{

struct Token
{
  TokenType   type;
  std::string str;
  size_t      line;
  size_t      column;
};

}  // namespace toy_c::token


#endif  // TOYC_TOKEN_H
