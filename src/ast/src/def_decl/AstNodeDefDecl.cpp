//
// Created by lzj on 2023.6.6.
//
#include "ast/def_decl/AstNodeDefDecl.h"
#include "ast/def_decl/TypeSpecifierAndQualifier.h"
#include "nlohmann/json_fwd.hpp"
#include <__msvc_iter_core.hpp>
#include <string_view>

namespace toy_c::ast
{


/**
 *
 * @return
 */
nlohmann::json DeclarationSpecifiers::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["typeSpecifier"]         = typeSpecifierMap.at(typeSpecifier);
  json["typeQualifier"]         = typeQualifierMap.at(typeQualifier);
  json["functionSpecifier"]     = functionSpecifierMap.at(functionSpecifier);
  json["storageClassSpecifier"] = storeClassSpecifierMap.at(storeClassSpecifier);
  json["isSigned"]              = isSigned;

  MEMBER_TO_JSON(typedefName)
  MEMBER_TO_JSON(structOrUnionSpecifier)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json Initializer::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(designation)
  MEMBER_TO_JSON(assignmentExpression)
  MEMBER_TO_JSON(initializerList)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json ArrayDeclarator::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignmentExpression)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json ArrayDeclaratorList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&] {
    auto list = nlohmann::json::array();
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
nlohmann::json ParameterDeclaration::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declarationDeclarator)
  MEMBER_TO_JSON(declarator)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json ParameterTypeList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(parameterList)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json DirectDeclarator::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(identifier)
  MEMBER_TO_JSON(parameterTypeList)
  MEMBER_TO_JSON(arrayDeclaratorList)
  MEMBER_TO_JSON(declarator)
  MEMBER_TO_JSON(identifierList)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json Declarator::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON


  MEMBER_TO_JSON(directDeclarator)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json InitialDeclarator::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declarator)
  MEMBER_TO_JSON(initializer)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json InitialDeclaratorList::toJson()
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
nlohmann::json DeclarationOrDefinition::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declarationDeclarator)
  MEMBER_TO_JSON(initialDeclaratorList)
  MEMBER_TO_JSON(compoundStatement)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json IdentifierList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  nlohmann::json list = nlohmann::json::array();
  for (auto i = begin(); i != end(); ++i)
  {
    list.push_back((*i)->toJson());
  }
  json["member"] = list;

  return json;
}
/**
 *
 * @return
 */
nlohmann::json ParameterList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&] {
    nlohmann::json list = nlohmann::json::array();
    for (auto i = begin(); i != end(); ++i)
    {
      list.push_back((*i)->toJson());
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json InitializerList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&] {
    nlohmann::json list = nlohmann::json::array();
    for (auto i : *this)
    {
      list.push_back([&] {
        nlohmann::json designationAndInitializer;
        auto           designation = std::get<0>(i);
        auto           initializer = std::get<1>(i);
        if (designation != nullptr)
        {
          designationAndInitializer["designation"] = designation->toJson();
        }
        if (initializer != nullptr)
        {
          designationAndInitializer["initializer"] = initializer->toJson();
        }
        return json;
      }());
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json TypeQualifierList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&] {
    nlohmann::json list = nlohmann::json::array();
    for (auto i : *this)
    {
      list.push_back(typeQualifierMap.at(i));
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json Pointer::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(typeQualifierList)
  MEMBER_TO_JSON(nextPointer)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json Designator::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignmentExpression)
  MEMBER_TO_JSON(identifier)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json Designator_list::toJson()
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
nlohmann::json Designation::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(designatorList)

  return json;
}
}  // namespace toy_c::ast
