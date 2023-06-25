//
// Created by lzj on 2023.6.5.
//

#ifndef TOYC_AST_NODE_EXPRESSION_H
#define TOYC_AST_NODE_EXPRESSION_H

#include "ast/AstNodeBasic.h"
#include "ast/AstNodeExpression.h"
#include "ast/AstNodeStatement.h"
#include "ast/def_decl/AstNodeDefDecl.h"
#include "token/token.h"
#include <variant>
#include <vector>

namespace toy_c::ast
{

struct Expression;


struct PostfixOperator : public AstNode
{
  token::TokenType postfixOperator = token::TokenType::invalid;
  Expression      *expression      = nullptr;

  nlohmann::json toJson() override;
};

struct PostfixOperatorList : public AstNode, public std::vector<PostfixOperator *>
{
  nlohmann::json toJson() override;
};

struct PrimaryExpression;
struct PostfixExpression : public AstNode
{
  PrimaryExpression   *primaryExpression   = nullptr;
  PostfixOperatorList *postfixOperatorList = nullptr;

  nlohmann::json toJson() override;
};

struct DeclarationSpecifiers;
struct UnaryExpression : public AstNode
{
  token::TokenType unaryOperator = token::TokenType::invalid;
  // DeclarationSpecifiers *declarationDeclarator = nullptr;
  UnaryExpression       *unaryExpression       = nullptr;
  PostfixExpression     *postfixExpression     = nullptr;
  DeclarationSpecifiers *declarationDeclarator = nullptr;

  bool isSizeof = false;

  nlohmann::json toJson() override;
};

struct BinaryExpression : public AstNode
{
  token::TokenType binaryOperator = token::TokenType::invalid;
  std::variant<UnaryExpression *, BinaryExpression *> leftExpr;
  std::variant<UnaryExpression *, BinaryExpression *> rightExpr;

  nlohmann::json toJson() override;
};

struct ConditionalExpression : public AstNode
{
  std::variant<UnaryExpression *, BinaryExpression *> unaryOrBinaryExpression;
  Expression                                         *expression            = nullptr;
  ConditionalExpression                              *conditionalExpression = nullptr;

  nlohmann::json toJson() override;
};

struct AssignmentExpression : public AstNode
{
  // assign
  // a = b + c = d * f - g
  token::TokenType      assignmentType           = token::TokenType::invalid;
  UnaryExpression      *unaryExpression          = nullptr;
  AssignmentExpression *nextAssignmentExpression = nullptr;

  // conditional expression
  ConditionalExpression *conditionalExpression = nullptr;

  // binary expression
  BinaryExpression *binaryExpression = nullptr;
  // unary expression

  nlohmann::json toJson() override;
};


struct LiteralInteger : public AstNode
{
  bool        isNegative = false;
  std::string value;

  nlohmann::json toJson() override;
};

struct LiteralFloat : public AstNode
{
  bool        isNegative = false;
  std::string value;

  nlohmann::json toJson() override;
};

struct LiteralString : public AstNode
{
  std::string value;

  nlohmann::json toJson() override;
};

struct LiteralChar : public AstNode
{
  char value;

  nlohmann::json toJson() override;
};

struct PrimaryExpression : public AstNode
{
  Identifier *identifier = nullptr;
  Expression *expression = nullptr;

  LiteralInteger *literalInteger = nullptr;
  LiteralFloat   *literalFloat   = nullptr;
  LiteralString  *literalString  = nullptr;
  LiteralChar    *literalChar    = nullptr;

  nlohmann::json toJson() override;
};

struct ArrayPostfixOperator : public PostfixOperator
{
  Expression *expression = nullptr;

  nlohmann::json toJson() override;
};

struct AssignmentExpressionList : public AstNode,
                                  public std::vector<AssignmentExpression *>
{
  nlohmann::json toJson() override;
};

struct FunctionPostfixOperator : public PostfixOperator
{
  AssignmentExpressionList *assignmentExpressionList = nullptr;

  nlohmann::json toJson() override;
};

struct MemberPostfixOperator : public PostfixOperator
{
  Identifier *identifier = nullptr;

  nlohmann::json toJson() override;
};

struct ArrowPostfixOperator : public PostfixOperator
{
  Identifier *identifier = nullptr;

  nlohmann::json toJson() override;
};

struct SelfPlusPostfixOperator : public PostfixOperator
{
  nlohmann::json toJson() override;
};

struct SelfMinusPostfixOperator : public PostfixOperator
{
  nlohmann::json toJson() override;
};

}  // namespace toy_c::ast

#endif  // TOYC_AST_NODE_EXPRESSION_H
