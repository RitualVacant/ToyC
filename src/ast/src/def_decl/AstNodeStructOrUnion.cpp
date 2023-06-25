//
// Created by lzj on 2023.6.22.
//
#include "ast/def_decl/AstNodeStructOrUnion.h"
#include "ast/AstNodeBasic.h"
#include "ast/def_decl/TypeSpecifierAndQualifier.h"
#include "macro/error.h"
#include <map>
#include <string_view>

namespace toy_c::ast
{

/**
 *
 * @return
 */
nlohmann::json StructOrUnionSpecifier::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  if (isStruct)
  {
    json["specifier"] = "struct";
  }
  else
  {
    json["specifier"] = "union";
  }

  MEMBER_TO_JSON(identifier)
  MEMBER_TO_JSON(structDeclarationList)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json StructDeclarationList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&] {
    nlohmann::json list = nlohmann::json::array();
    for (auto i : *this)
    {
      list.push_back(i->toJson());
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json StructDeclaration::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(specifierQualifierList)
  MEMBER_TO_JSON(structDeclaratorList)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json SpecifierQualifierList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["isSigned"]          = isSigned;
  json["typeQualifierList"] = [&] {
    nlohmann::json list = nlohmann::json::array();
    for (auto i : typeQualifierList)
    {
      list.push_back(typeQualifierMap.at(i));
    }
    return list;
  }();
  json["typeSpecifier"] = typeSpecifierMap.at(typeSpecifier);

  return json;
}

/**
 *
 * @return
 */
nlohmann::json StructDeclaratorList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&] {
    nlohmann::json list = nlohmann::json::array();
    for (auto i : *this)
    {
      list.push_back(i->toJson());
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json StructDeclarator::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declarator)
  MEMBER_TO_JSON(assignmentExpression)

  return json;
}

}  // namespace toy_c::ast