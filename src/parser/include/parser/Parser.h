//
// Created by lzj on 2023.6.5.
//

#ifndef TOYC_PARSER_H
#define TOYC_PARSER_H

#include "ast/AstNode.h"
#include "parser/ParserImpl.h"
#include "scan/scanner.h"

namespace toy_c::parser
{

class Parser : public ParserImpl
{
public:
  Parser();
  ~Parser() = default;

  nlohmann::json toJson();
  void           genSpec();

  // parser
};

}  // namespace toy_c::parser


#endif  // TOYC_PARSER_H
