//
// Created by lzj on 2023.6.4.
//

#ifndef TOYC_SCANNER_H
#define TOYC_SCANNER_H

#include "scan/ScannerInputFile.h"
#include "token/Token.h"
#include "token/TokenType.h"
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <tuple>

namespace toy_c::scan
{


class Scanner
{
public:
  Scanner();
  ~Scanner() = default;

  void             nextToken();
  void             eatCurrentToken(token::TokenType token_type_);
  token::Token     getCurrentToken();
  std::string      getCurrentTokenStr();
  token::TokenType getCurrentTokenType();

  token::Token     getNextToken();
  token::TokenType getNextTokenType();
  std::string      getNextTokenStr();

  bool isCompleted();

private:
  ScannerInputFile m_inputFile;

  token::Token m_currentToken;
  token::Token m_nextToken;
  bool         m_isReallyNextToken = true;

  token::Token to_number(bool is_negative_);
  token::Token to_keyWord_or_identify();
};

}  // namespace toy_c::scan

#endif  // TOYC_SCANNER_H