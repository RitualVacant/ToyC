//
// Created by lzj on 2023.6.5.
//
#include "ast/AstNodeExpression.h"
#include "macro/error.h"
#include <cstdio>

namespace toy_c::ast
{


/**
 *
 * @return
 */
nlohmann::json PrimaryExpression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(identifier)
  MEMBER_TO_JSON(expression)
  MEMBER_TO_JSON(literalInteger)
  MEMBER_TO_JSON(literalFloat)
  MEMBER_TO_JSON(literalString)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json PostfixExpression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(primaryExpression)
  MEMBER_TO_JSON(postfixOperatorList)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json UnaryExpression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["isSizeof"] = isSizeof;

  MEMBER_TOKEN_TO_JSON(unaryOperator)
  MEMBER_TO_JSON(unaryExpression)
  MEMBER_TO_JSON(postfixExpression)
  MEMBER_TO_JSON(declarationDeclarator)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json BinaryExpression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TOKEN_TO_JSON(binaryOperator)

  if (std::get_if<UnaryExpression *>(&leftExpr) != nullptr)
  {
    auto *leftUnaryExpr = std::get<UnaryExpression *>(leftExpr);
    MEMBER_TO_JSON(leftUnaryExpr)
  }
  else
  {
    auto *rightBinaryExpr = std::get<BinaryExpression *>(leftExpr);
    MEMBER_TO_JSON(rightBinaryExpr)
  }

  if (std::get_if<UnaryExpression *>(&rightExpr) != nullptr)
  {
    auto *rightUnaryExpr = std::get<UnaryExpression *>(rightExpr);
    MEMBER_TO_JSON(rightUnaryExpr)
  }
  else
  {
    auto *rightBinaryExpr = std::get<BinaryExpression *>(rightExpr);
    MEMBER_TO_JSON(rightBinaryExpr)
  }

  return json;
}
/**
 *
 * @return
 */
nlohmann::json ConditionalExpression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  if (std::get_if<UnaryExpression *>(&unaryOrBinaryExpression) != nullptr)
  {
    auto unaryExpression = std::get<UnaryExpression *>(unaryOrBinaryExpression);
    MEMBER_TO_JSON(unaryExpression)
  }
  else
  {
    auto binaryExpression = std::get<BinaryExpression *>(unaryOrBinaryExpression);
    MEMBER_TO_JSON(binaryExpression)
  }
  MEMBER_TO_JSON(expression)
  MEMBER_TO_JSON(conditionalExpression)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json AssignmentExpression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TOKEN_TO_JSON(assignmentType)

  MEMBER_TO_JSON(unaryExpression)
  MEMBER_TO_JSON(nextAssignmentExpression)

  MEMBER_TO_JSON(conditionalExpression)
  MEMBER_TO_JSON(binaryExpression)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json Expression::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&]() {
    nlohmann::json list = nlohmann::json::array();
    for (auto item : *this)
    {
      list.push_back(item->toJson());
    }
    return list;
  }();

  return json;
}
/**
 *
 * @return
 */
nlohmann::json PostfixOperatorList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&]() {
    nlohmann::json list = nlohmann::json::array();
    for (auto item : *this)
    {
      list.push_back(item->toJson());
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json toy_c::ast::PostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  MEMBER_TOKEN_TO_JSON(postfixOperator)
  MEMBER_TO_JSON(expression)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json toy_c::ast::LiteralInteger::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON
  json["isNegative"] = isNegative;
  json["value"]      = value;

  return json;
}

/**
 *
 * @return
 */
nlohmann::json toy_c::ast::LiteralFloat::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON
  json["isNegative"] = isNegative;
  json["value"]      = value;

  return json;
}

/**
 *
 * @return
 */
nlohmann::json toy_c::ast::LiteralString::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON
  json["value"] = value;

  return json;
}
/**
 *
 * @return
 */
nlohmann::json ArrayPostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(expression)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json AssignmentExpressionList::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  json["list"] = [&]() {
    nlohmann::json list = nlohmann::json::array();
    for (auto item : *this)
    {
      list.push_back(item->toJson());
    }
    return list;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json FunctionPostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignmentExpressionList)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json MemberPostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(identifier)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json ArrowPostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(identifier)

  return json;
}


/**
 *
 * @return
 */
nlohmann::json SelfPlusPostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  return json;
}

/**
 *
 * @return
 */
nlohmann::json SelfMinusPostfixOperator::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  return json;
}

/**
 *
 * @return
 */
nlohmann::json LiteralChar::toJson()
{
  nlohmann::json json;

  AST_TYPE_TO_JSON

  json["value"] = value;

  return json;
}

}  // namespace toy_c::ast
