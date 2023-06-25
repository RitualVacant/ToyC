//
// Created by lzj on 2023.6.5.
//
#include "ast/AstNodeStatement.h"
#include "ast/AstNodeBasic.h"
#include "macro/error.h"

namespace toy_c::ast
{


/**
 *
 * @return
 */
nlohmann::json IfStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignExpression)
  MEMBER_TO_JSON(ifBody)
  MEMBER_TO_JSON(elseBody)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json CompoundStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(blockList)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json WhileStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignmentExpression)
  MEMBER_TO_JSON(body)

  return json;
}
/**
 *
 * @return
 */
nlohmann::json DoWhileStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignExpression)
  MEMBER_TO_JSON(compoundStatement)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json ForStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declaration)
  MEMBER_TO_JSON(conditionalAssignExpression)
  MEMBER_TO_JSON(changeAssignExpression)
  MEMBER_TO_JSON(compoundStatement)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json BreakStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  return json;
}

/**
 *
 * @return
 */
nlohmann::json SwitchStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(assignExpression)
  MEMBER_TO_JSON(compoundStatement)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json GotoStatement::toJson()
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
nlohmann::json ContinueStatement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  return json;
}

/**
 *
 * @return
 */
nlohmann::json ReturnStatement::toJson()
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
nlohmann::json BlockList::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  json["list"] = [&]() -> nlohmann::json {
    nlohmann::json json = nlohmann::json::array();
    for (auto block : *this)
    {
      json.push_back(block->toJson());
    }
    return json;
  }();

  return json;
}

/**
 *
 * @return
 */
nlohmann::json Block::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declaration)
  MEMBER_TO_JSON(statement)

  return json;
}

/**
 *
 * @return
 */
nlohmann::json CaseLabel::toJson()
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
nlohmann::json DefaultLabel::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  return json;
}

/**
 *
 * @return
 */
nlohmann::json Label::toJson()
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
nlohmann::json Statement::toJson()
{
  nlohmann::json json;
  AST_TYPE_TO_JSON

  MEMBER_TO_JSON(declarationOrDefinition)

  return json;
}

}  // namespace toy_c::ast
