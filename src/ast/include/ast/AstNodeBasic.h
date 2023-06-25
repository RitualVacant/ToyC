//
// Created by lzj on 2023.6.5.
//

#ifndef TOYC_AST_NODE_BASIC_H
#define TOYC_AST_NODE_BASIC_H


#include <nlohmann/json.hpp>
#include <string>


#define MEMBER_TO_JSON(ptr_)                                                             \
  if (ptr_ != nullptr)                                                                   \
  {                                                                                      \
    json[#ptr_] = ptr_->toJson();                                                        \
  }

#define MEMBER_TOKEN_TO_JSON(token_)                                                     \
  if (token_ != token::TokenType::invalid)                                               \
  {                                                                                      \
    json[#token_] = token::c_TokenType_map_string.at(token_);                            \
  }

#define AST_TYPE_TO_JSON                                                                 \
  std::string_view sv(typeid(*this).name());                                             \
  sv.remove_prefix(sv.rfind(':') + 1);                                                   \
  json["type"] = sv;

namespace toy_c::ast
{

struct AstNode
{
  size_t line;
  size_t column;

  virtual nlohmann::json toJson() = 0;
};

struct Identifier : public AstNode
{
  std::string    name;
  nlohmann::json toJson() override;
};


}  // namespace toy_c::ast
#endif  // TOYC_AST_NODE_BASIC_H
