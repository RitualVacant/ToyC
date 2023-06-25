//
// Created by lzj on 2023.6.5.
//

#ifndef TOYC_AST_NODE_STATEMENT_H
#define TOYC_AST_NODE_STATEMENT_H

#include "ast/AstNodeBasic.h"
#include "ast/AstNodeExpression.h"
#include "ast/def_decl/AstNodeDefDecl.h"

namespace toy_c::ast
{

struct DeclarationOrDefinition;
struct AssignmentExpression;


//////////////////////////////////////////////////////////////////////////////////////////
struct Statement : public AstNode
{
  DeclarationOrDefinition *declarationOrDefinition = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
//// label statement
//////////////////////////////////////////////////////////////////////////////////////////
struct Label : public Statement
{
  Identifier *identifier = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct CaseLabel : public Statement
{
  Identifier *identifier = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct DefaultLabel : public Statement
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
//// compound statement
//////////////////////////////////////////////////////////////////////////////////////////
struct Block : public AstNode
{
  DeclarationOrDefinition *declaration = nullptr;
  Statement               *statement   = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct BlockList : public AstNode, public std::vector<Block *>
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct CompoundStatement : public Statement
{
  BlockList *blockList = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
//// selection statement
//////////////////////////////////////////////////////////////////////////////////////////
struct IfStatement : public Statement
{
  AssignmentExpression *assignExpression = nullptr;
  Statement            *ifBody           = nullptr;
  Statement            *elseBody         = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct SwitchStatement : public Statement
{
  AssignmentExpression *assignExpression  = nullptr;
  CompoundStatement    *compoundStatement = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
//// iteration statement
//////////////////////////////////////////////////////////////////////////////////////////
struct WhileStatement : public Statement
{
  AssignmentExpression *assignmentExpression = nullptr;
  CompoundStatement    *body                 = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct DoWhileStatement : public Statement
{
  AssignmentExpression *assignExpression  = nullptr;
  CompoundStatement    *compoundStatement = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ForStatement : public Statement
{
  DeclarationOrDefinition *declaration                 = nullptr;
  AssignmentExpression    *conditionalAssignExpression = nullptr;
  AssignmentExpression    *changeAssignExpression      = nullptr;
  CompoundStatement       *compoundStatement           = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
//// jump statement
//////////////////////////////////////////////////////////////////////////////////////////
struct BreakStatement : public Statement
{
  nlohmann::json toJson() override;
};


//////////////////////////////////////////////////////////////////////////////////////////
struct GotoStatement : public Statement
{
  Identifier *identifier = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ContinueStatement : public Statement
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ReturnStatement : public Statement
{
  AssignmentExpression *assignmentExpression = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
//// expression statement
//////////////////////////////////////////////////////////////////////////////////////////
struct Expression : public std::vector<AssignmentExpression *>, public Statement
{
  nlohmann::json toJson() override;
};

}  // namespace toy_c::ast

#endif  // TOYC_AST_NODE_H
