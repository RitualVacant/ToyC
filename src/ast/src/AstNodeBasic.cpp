//
// Created by lzj on 2023.6.5.
//
#include "ast/AstNodeBasic.h"
#include "fmt/core.h"

namespace toy_c::ast
{

nlohmann::json Identifier::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON
  json["name"] = name;

  return json;
}

}  // namespace toy_c::ast