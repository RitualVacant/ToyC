//
// Created by lzj on 2023.6.7.
//
#include "parser/ParserImpl.h"
#include "ast/AstNodeExpression.h"
#include "ast/AstNodeStatement.h"
#include "ast/def_decl/AstNodeDefDecl.h"
#include "ast/def_decl/TypeSpecifierAndQualifier.h"
#include "macro/error.h"
#include "parser/error.h"
#include "token/TokenType.h"
#include <fmt/color.h>
#include <vector>

namespace toy_c::parser
{

/*
 *
 */
ParserImpl::ParserImpl()
{
  m_scanner.nextToken();
  while (!m_scanner.isCompleted())
  {
    m_translationUnit.push_back(parser_declaration_or_definition());
  }
}

/**
 *
 * @return
 */
ast::TranslationUnit *ParserImpl::parser_translation_unit()
{
  m_scanner.nextToken();
  while (!m_scanner.isCompleted())
  {
    m_translationUnit.push_back(parser_declaration_or_definition());
  }
  return nullptr;
}


/**
 *
 * @return
 *
 */
ast::DeclarationOrDefinition *ParserImpl::parser_declaration_or_definition()
{
  /*
   * declaration_or_definition
   * (1) -> declaration_declarator ;
   * (2)  | declaration_declarator initial_declarator_list ;
   * (3)  | declaration_declarator initial_declarator_list {..}
   *
   * (1) struct union enum declaration
   * (2) var
   * (3) function
   */

  auto *declarationOrDefinition = new ast::DeclarationOrDefinition;

  declarationOrDefinition->declarationDeclarator = parser_declaration_specifiers();

  // (1)
  if (m_scanner.getCurrentTokenType() == token::TokenType::semicolon)
  {
    m_scanner.eatCurrentToken(token::TokenType::semicolon);
  }
  // (2) (3)
  else
  {
    declarationOrDefinition->initialDeclaratorList = parser_initial_declarator_list();

    switch (m_scanner.getCurrentTokenType())
    {
      // (2)
      case token::TokenType::semicolon: {
        m_scanner.eatCurrentToken(token::TokenType::semicolon);
        break;
      }
      // (3)
      case token::TokenType::l_big_par: {
        declarationOrDefinition->compoundStatement = parser_compound_statement();
        break;
      }
      default: {
        auto expected_token_type
          = {token::TokenType::semicolon, token::TokenType::l_big_par};
        ERROR_UNEXPECT_TOKEN_2arg(m_scanner.getCurrentToken(), expected_token_type)
      }
    }
  }

  return declarationOrDefinition;
}

/**
 *
 * @return
 */
ast::DeclarationSpecifiers *ParserImpl::parser_declaration_specifiers()
{
  auto *declarationDeclarator = new ast::DeclarationSpecifiers;
  bool  isContinue            = true;

  // because the order of C's declarator is random
  // this loop will eat token until get identifier or other token
  while (isContinue)
  {
    switch (m_scanner.getCurrentTokenType())
    {
      //
      // storage class specifier
      //
      case token::TokenType::key_typedef: {
        declarationDeclarator->storeClassSpecifier = ast::StoreClassSpecifier::_typedef_;
        m_scanner.eatCurrentToken(token::TokenType::key_typedef);
        declarationDeclarator->typedefName = parser_identifier();
        break;
      }
      case token::TokenType::key_extern: {
        declarationDeclarator->storeClassSpecifier = ast::StoreClassSpecifier::_extern_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_static: {
        declarationDeclarator->storeClassSpecifier = ast::StoreClassSpecifier::_static_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_auto:
      case token::TokenType::key_register: {
        m_scanner.nextToken();
        break;
      }
      //
      // type specifier
      //
      case token::TokenType::key_void: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_void_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_char: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_char_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_short: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_short_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_int: {
        if (auto &typeSpecifier = declarationDeclarator->typeSpecifier;
            typeSpecifier != ast::TypeSpecifier::_long_
            || typeSpecifier != ast::TypeSpecifier::_long_long_)
        {
          typeSpecifier = ast::TypeSpecifier::_int_;
        }
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_long: {
        auto &typeSpecifier = declarationDeclarator->typeSpecifier;
        if (typeSpecifier == ast::TypeSpecifier::_int_)
        {
          typeSpecifier = ast::TypeSpecifier::_long_;
        }
        else if (typeSpecifier == ast::TypeSpecifier::_long_)
        {
          typeSpecifier = ast::TypeSpecifier::_long_long_;
        }
        else
        {
          typeSpecifier = ast::TypeSpecifier::_long_;
        }
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_float: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_float_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_double: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_double_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_signed: {
        declarationDeclarator->isSigned = true;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_unsigned: {
        declarationDeclarator->isSigned = false;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key__Bool: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::__Bool_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key__Complex: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::__Complex_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_struct: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_struct_;
        declarationDeclarator->structOrUnionSpecifier
          = parser_struct_or_union_specifier();
        return declarationDeclarator;
      }
      case token::TokenType::key_union: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_union_;
        declarationDeclarator->structOrUnionSpecifier
          = parser_struct_or_union_specifier();
        return declarationDeclarator;
      }
      case token::TokenType::key_enum: {
        declarationDeclarator->typeSpecifier = ast::TypeSpecifier::_enum_;
        return declarationDeclarator;
      }
        //
        // type qualifier
        //
      case token::TokenType::key_restrict: {
        declarationDeclarator->typeQualifier = ast::TypeQualifier::_restrict_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_volatile: {
        declarationDeclarator->typeQualifier = ast::TypeQualifier::_volatile_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_const: {
        declarationDeclarator->typeQualifier = ast::TypeQualifier::_const_;
        m_scanner.nextToken();
        break;
      }
        //
        // function specifier
        //
      case token::TokenType::key_inline: {
        declarationDeclarator->functionSpecifier = ast::FunctionSpecifier::_inline_;
        m_scanner.nextToken();
        break;
      }
      default:
        isContinue = false;
    }
  }

  return declarationDeclarator;
}

/**
 *
 * @return
 */
ast::InitialDeclaratorList *ParserImpl::parser_initial_declarator_list()
{
  /*
   * initial_declarator_list
   * (1) -> initial_declarator
   * (2)  | initial_declarator , initial_declarator_list
   */
  auto *initialDeclaratorList = new ast::InitialDeclaratorList;
  bool  isContinue            = true;

  do
  {
    initialDeclaratorList->push_back(parser_initial_declarator());
    if (m_scanner.getCurrentTokenType() == token::TokenType::comma)
    {
      m_scanner.eatCurrentToken(token::TokenType::comma);
    }
    else
    {
      isContinue = false;
    }
  } while (isContinue);

  return initialDeclaratorList;
}

/**
 *
 * @return
 */
ast::InitialDeclarator *ParserImpl::parser_initial_declarator()
{
  /*
   * initial_declarator
   * (1) -> declarator
   * (2)  | declarator = initializer
   */
  auto *initialDeclarator = new ast::InitialDeclarator;

  // (1)
  initialDeclarator->declarator = parser_declarator();

  // (2)
  if (m_scanner.getCurrentTokenType() == token::TokenType::assign)
  {
    m_scanner.nextToken();
    initialDeclarator->initializer = parser_initializer();
  }

  return initialDeclarator;
}

/**
 *
 * @return
 */
ast::Declarator *ParserImpl::parser_declarator()
{
  /*
   * declarator
   * (1) -> direct_declarator
   * (2)  | pointer direct_declarator
   *
   * pointer can be empty, so we can't use direct_declarator to parser
   * when pointer is empty, parser_pointer() will return nullptr
   * pointer begin with * or * const or * const volatile
   * if current token isn't *, parser_pointer() will return nullptr
   */

  auto *declarator = new ast::Declarator;

  declarator->pointer          = parser_pointer();
  declarator->directDeclarator = parser_direct_declarator();

  return declarator;
}

/**
 *
 * @return
 */
ast::Initializer *ParserImpl::parser_initializer()
{
  /*
   * initializer
   * -> assignment-expression
   *  | { initializer-list }
   *  | { initializer-list , }
   *                       |
   *                       |
   *                       +-> this , had been eaten by parser_initializer_list()
   */

  auto *initializer = new ast::Initializer;

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::l_big_par: {
      m_scanner.eatCurrentToken(token::TokenType::l_big_par);
      initializer->initializerList = parser_initializer_list();
      m_scanner.eatCurrentToken(token::TokenType::r_big_par);
      break;
    }
    default:
      initializer->assignmentExpression = parser_assignment_expression();
  }

  return initializer;
}

/**
 *
 * @return
 */
ast::InitializerList *ParserImpl::parser_initializer_list()
{
  /*
   * initializer_list
   * (1) -> initializer
   * (2)  | designation initializer
   * (3)  | initializer, initializer_list
   * (4)  | designation initializer, initializer_list
   *
   * designation begin with [ or .
   * when current token isn't [ or .
   * parser_designation() will return nullptr
   */

  auto *initializerList = new ast::InitializerList;
  bool  isContinue      = true;

  do
  {
    initializerList->push_back({parser_designation(), parser_initializer()});

    if (m_scanner.getCurrentTokenType() == token::TokenType::comma)
    {
      m_scanner.eatCurrentToken(token::TokenType::comma);
      if (m_scanner.getCurrentTokenType() == token::TokenType::r_big_par)
      {
        isContinue = false;
      }
    }
    else
    {
      isContinue = false;
    }
  } while (isContinue);

  return initializerList;
}

/**
 *
 * @return
 */
ast::Designation *ParserImpl::parser_designation()
{
  /*
   * designation
   * (1) -> designator_list =
   *
   * designator_list begin with [ or .
   * when current token isn't [ or .
   * parser_designator_list() will return nullptr
   */

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::l_mid_par:
    case token::TokenType::dot: {
      auto *designation           = new ast::Designation;
      designation->designatorList = parser_designator_list();
      m_scanner.eatCurrentToken(token::TokenType::assign);
      return designation;
    }

    default:
      return nullptr;
  }
  NOT_REACHABLE
}

/**
 *
 * @return
 */
ast::Designator_list *ParserImpl::parser_designator_list()
{
  auto *designatorList = new ast::Designator_list;

  while (true)
  {
    switch (m_scanner.getCurrentTokenType())
    {
      case token::TokenType::l_mid_par:
      case token::TokenType::dot:
        designatorList->push_back(parser_designator());
      default:
        return designatorList;
    }
  }

  NOT_REACHABLE
}

/**
 *
 * @return
 */
ast::Designator *ParserImpl::parser_designator()
{
  auto *designator = new ast::Designator;
  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::l_mid_par: {
      m_scanner.eatCurrentToken(token::TokenType::l_mid_par);
      designator->assignmentExpression = parser_assignment_expression();
      m_scanner.eatCurrentToken(token::TokenType::r_mid_par);
      break;
    }
    case token::TokenType::dot: {
      m_scanner.eatCurrentToken(token::TokenType::dot);
      designator->identifier = parser_identifier();
      break;
    }
    default:
      auto expectTokenList = {token::TokenType::l_mid_par, token::TokenType::dot};
      ERROR_UNEXPECT_TOKEN_2arg(m_scanner.getCurrentToken(), expectTokenList)
  }
  return designator;
}

/**
 *
 * @return
 */
ast::DirectDeclarator *ParserImpl::parser_direct_declarator()
{
  /*
   * direct_declarator
   * (1) -> identify
   * (2)  | identify ( parameter_type_list | identify_list | )
   * (3)  | identify array_declarator_list
   * (4)  | ( declarator ) array_declarator_list
   * (5)  | ( declarator ) ( parameter_type_list | identify_list | )
   *
   *  abstract_declarator can be empty, but still use direct_declarator to parser,
   *  abstract_declarator without identifier
   *  (6) |
   *  (7) | ( parameter_type_list | identify_list | )
   *  (8) | array_declarator_list
   */

  auto *directDeclarator = new ast::DirectDeclarator;

  // (1) (2) (3)
  switch (m_scanner.getCurrentTokenType())
  {
    // (1) (2) (3)
    case token::TokenType::identify: {
      directDeclarator->identifier = parser_identifier();
      switch (m_scanner.getCurrentTokenType())
      {
        // (2)
        case token::TokenType::l_par: {
          m_scanner.eatCurrentToken(token::TokenType::l_par);
          switch (m_scanner.getCurrentTokenType())
          {
            case token::TokenType::r_par:
              break;
            case token::TokenType::identify: {
              directDeclarator->identifierList = parser_identifier_list();
              fmt::print(
                fg(fmt::color::red),
                "[ERROR]: K&R-style function definition\n"
                "identifier list  in direct declarator is not support\n"
              );
              exit(1);
            }
            default:
              directDeclarator->parameterTypeList = parser_parameter_type_list();
          }
          m_scanner.eatCurrentToken(token::TokenType::r_par);
          break;
        }
        // (3)
        case token::TokenType::l_mid_par: {
          directDeclarator->arrayDeclaratorList = parser_array_declarator_list();
          break;
        }
        default:
          break;
      }
      break;
    }
    // (4) (5) (7)
    case token::TokenType::l_par: {
      m_scanner.eatCurrentToken(token::TokenType::l_par);

      switch (m_scanner.getCurrentTokenType())
      {
        // (7)
        case token::TokenType::key_void:
        case token::TokenType::key_char:
        case token::TokenType::key_short:
        case token::TokenType::key_int:
        case token::TokenType::key_long:
        case token::TokenType::key_float:
        case token::TokenType::key_double:
        case token::TokenType::key__Bool:
        case token::TokenType::key__Complex:
        case token::TokenType::key_signed:
        case token::TokenType::key_unsigned:
        case token::TokenType::key_struct:
        case token::TokenType::key_union:
        case token::TokenType::key_enum: {
          directDeclarator->parameterTypeList = parser_parameter_type_list();
          break;
        }
        // (7)
        case token::TokenType::identify: {
          NOT_REACHABLE  // don't support K&R-style function definition now
            directDeclarator->identifierList
            = parser_identifier_list();
          break;
        }
        // (4) (5)
        default: {
          directDeclarator->declarator = parser_declarator();
          m_scanner.eatCurrentToken(token::TokenType::r_par);

          switch (m_scanner.getCurrentTokenType())
          {
            // (5)
            case token::TokenType::l_par: {
              m_scanner.eatCurrentToken(token::TokenType::l_par);
              switch (m_scanner.getCurrentTokenType())
              {
                case token::TokenType::r_par:
                  break;
                case token::TokenType::identify: {
                  directDeclarator->identifierList = parser_identifier_list();
                  fmt::print(
                    fg(fmt::color::red),
                    "[ERROR]: K&R-style function definition\n"
                    "identifier list  in direct declarator is not support\n"
                  );
                  exit(1);
                }
                default:
                  directDeclarator->parameterTypeList = parser_parameter_type_list();
              }
              m_scanner.eatCurrentToken(token::TokenType::r_par);
              break;
            }
              // (4)
            case token::TokenType::l_mid_par: {
              directDeclarator->arrayDeclaratorList = parser_array_declarator_list();
              break;
            }
            default:
              break;
          }
        }
      }
      break;
    }
    // (6) end
    case token::TokenType::comma:
    case token::TokenType::r_par:
    case token::TokenType::r_big_par: {
      delete directDeclarator;
      return nullptr;
    }
    // (8)
    case token::TokenType::l_mid_par: {
      directDeclarator->arrayDeclaratorList = parser_array_declarator_list();
      break;
    }
    default: {
      auto expected_token_type = {token::TokenType::identify, token::TokenType::l_par};
      ERROR_UNEXPECT_TOKEN_2arg(m_scanner.getCurrentToken(), expected_token_type)
    }
  }

  return directDeclarator;
}

/**
 *
 * @return
 */
ast::Identifier *ParserImpl::parser_identifier()
{
  auto *identifier = new ast::Identifier;

  identifier->name = m_scanner.getCurrentTokenStr();
  m_scanner.nextToken();

  return identifier;
}

/**
 *
 * @return
 */
ast::ParameterTypeList *ParserImpl::parser_parameter_type_list()
{
  /*
   *  parameter_type_list
   *  -> parameter_list
   *   | parameter_list , ...
   */

  auto *parameterTypeList = new ast::ParameterTypeList;

  parameterTypeList->parameterList = parser_parameter_list();
  if (m_scanner.getCurrentTokenType() == token::TokenType::comma)
  {
    m_scanner.eatCurrentToken(token::TokenType::comma);
    m_scanner.eatCurrentToken(token::TokenType::ellipsis);
    parameterTypeList->isVariableParameter = true;
  }

  return parameterTypeList;
}

/**
 *
 * @return
 */
ast::ArrayDeclarator *ParserImpl::parser_array_declarator()
{
  auto *arrayDeclarator = new ast::ArrayDeclarator;

  m_scanner.eatCurrentToken(token::TokenType::l_mid_par);
  arrayDeclarator->assignmentExpression = parser_assignment_expression();
  m_scanner.eatCurrentToken(token::TokenType::r_mid_par);

  return arrayDeclarator;
}

/**
 *
 * @return
 */
ast::ArrayDeclaratorList *ParserImpl::parser_array_declarator_list()
{
  auto *arrayDeclaratorList = new ast::ArrayDeclaratorList;

  while (m_scanner.getCurrentTokenType() == token::TokenType::l_mid_par)
  {
    arrayDeclaratorList->push_back(parser_array_declarator());
  }

  return arrayDeclaratorList;
}

/**
 *
 * @return
 */
ast::ParameterList *ParserImpl::parser_parameter_list()
{
  /*
   * parameter_list
   * -> parameter_declaration , parameter_list
   */

  auto *parameterList = new ast::ParameterList;
  bool  isContinue    = true;

  do
  {
    parameterList->push_back(parser_parameter_declaration());
    if (m_scanner.getCurrentTokenType() == token::TokenType::comma)
    {
      if (m_scanner.getNextTokenType() == token::TokenType::ellipsis)
      {
        isContinue = false;
      }
      else
      {
        m_scanner.eatCurrentToken(token::TokenType::comma);
      }
    }
    else
    {
      isContinue = false;
    }
  } while (isContinue);

  return parameterList;
}

/**
 *
 * @return
 */
ast::IdentifierList *ParserImpl::parser_identifier_list()
{
  /*
   * (1) identifier_list -> identify
   * (2)                  | identify , identifier_list
   */

  auto *identifierList = new ast::IdentifierList;

  identifierList->push_back(parser_identifier());

  while (m_scanner.getCurrentTokenType() == token::TokenType::comma)
  {
    m_scanner.eatCurrentToken(token::TokenType::comma);
    identifierList->push_back(parser_identifier());
  }

  return identifierList;
}

/**
 *
 * @return
 */
ast::ParameterDeclaration *ParserImpl::parser_parameter_declaration()
{
  /*
   * arguments_declaration
   * -> declaration_declarator declarator
   */
  auto *parameterDeclaration = new ast::ParameterDeclaration;

  parameterDeclaration->declarationDeclarator = parser_declaration_specifiers();
  parameterDeclaration->declarator            = parser_declarator();

  return parameterDeclaration;
}

/**
 *
 * @return
 */
ast::Statement *ParserImpl::parser_statement()
{
  /*
   * statement
   * -> compound_statement
   *    -> { block_item_list }
   *     | { }
   *  | expression_statement
   *    -> expression ;
   *     | ;
   *  | label_statement
   *    -> identifier : statement
   *     | case constant_expression : statement
   *     | default : statement
   *  | selection_statement
   *    -> if ( expression ) statement
   *     | if ( expression ) statement else statement
   *     | switch ( expression ) statement
   *  | iteration_statement
   *    -> while ( expression ) statement
   *     | do statement while ( expression ) ;
   *     | for ( expression ; expression ; expression ) statement
   *     | for ( declaration ; expression ; expression ) statement
   *  | jump_statement
   *    -> goto identifier ;
   *     | continue ;
   *     | break ;
   *     | return expression ;
   */

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::l_big_par: {
      return parser_compound_statement();
    }
    case token::TokenType::key_int:
    case token::TokenType::key_double:
    case token::TokenType::key_long:
    case token::TokenType::key_char:
    case token::TokenType::key_float:
    case token::TokenType::key_struct:
    case token::TokenType::key__Bool:
    case token::TokenType::key__Complex:
    case token::TokenType::key__Imaginary:
    case token::TokenType::key_unsigned:
    case token::TokenType::key_signed: {
      auto *statement                    = new ast::Statement;
      statement->declarationOrDefinition = parser_declaration_or_definition();
      return statement;
    }

    case token::TokenType::key_case:
      return parser_case_label();
      break;

    case token::TokenType::key_default:
      return parser_default_label();
      break;

    case token::TokenType::identify: {
      if (m_scanner.getNextTokenType() == token::TokenType::colon)
      {
        return parser_label();
      }
      else
      {
        auto *expression = parser_expression();
        m_scanner.eatCurrentToken(token::TokenType::semicolon);
        return expression;
      }
    }

    case token::TokenType::key_if:
      return parser_if_statement();
    case token::TokenType::key_while:
      return parser_while_statement();
    case token::TokenType::key_switch:
      return parser_switch_statement();
    case token::TokenType::key_do:
      return parser_do_while_statement();
    case token::TokenType::key_for:
      return parser_for_statement();
    case token::TokenType::key_break:
      return parser_break_statement();
    case token::TokenType::key_continue:
      return parser_continue_statement();
    case token::TokenType::key_return:
      return parser_return_statement();
    case token::TokenType::key_goto:
      return parser_goto_statement();
    case token::TokenType::r_big_par:
      return nullptr;
    default:
      ERROR_UNEXPECT_TOKEN_1arg(m_scanner.getCurrentToken());
  }
  NOT_REACHABLE
}

/**
 *
 * @return
 */
ast::CompoundStatement *ParserImpl::parser_compound_statement()
{
  /*
   * (1) compound_statement -> { block_list }
   */
  auto *compoundStatement = new ast::CompoundStatement;

  m_scanner.eatCurrentToken(token::TokenType::l_big_par);
  compoundStatement->blockList = parser_block_list();
  m_scanner.eatCurrentToken(token::TokenType::r_big_par);

  return compoundStatement;
}

/**
 *
 * @return
 */
ast::BlockList *ParserImpl::parser_block_list()
{
  /*
   * (1) block_list -> block
   *                 | block block_list
   */
  auto *blockList = new ast::BlockList;

  while (m_scanner.getCurrentTokenType() != token::TokenType::r_big_par)
  {
    blockList->push_back(parser_block());
  }

  return blockList;
}

/**
 *
 * @return
 */
ast::Block *ParserImpl::parser_block()
{
  /*
   * block -> declaration
   *        | mark_statement
   *        | not_mark_statement
   */

  auto *block = new ast::Block;

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::key_int:
    case token::TokenType::key_double:
    case token::TokenType::key_long:
    case token::TokenType::key_char:
    case token::TokenType::key_float:
    case token::TokenType::key_struct:
    case token::TokenType::key__Bool:
    case token::TokenType::key__Complex:
    case token::TokenType::key__Imaginary:
    case token::TokenType::key_unsigned:
    case token::TokenType::key_signed:
      block->declaration = parser_declaration_or_definition();
      break;

    case token::TokenType::key_case:
      block->statement = parser_case_label();
      break;

    case token::TokenType::key_default:
      block->statement = parser_default_label();
      break;

    case token::TokenType::identify: {
      if (m_scanner.getNextTokenType() == token::TokenType::colon)
      {
        block->statement = parser_label();
      }
      else
      {
        block->statement = parser_expression();
        m_scanner.eatCurrentToken(token::TokenType::semicolon);
      }
      break;
    }
    case token::TokenType::key_if:
      block->statement = parser_if_statement();
      break;
    case token::TokenType::key_while:
      block->statement = parser_while_statement();
      break;
    case token::TokenType::key_switch:
      block->statement = parser_switch_statement();
      break;
    case token::TokenType::key_do:
      block->statement = parser_do_while_statement();
      break;
    case token::TokenType::key_for:
      block->statement = parser_for_statement();
      break;
    case token::TokenType::key_break:
      block->statement = parser_break_statement();
      break;
    case token::TokenType::key_continue:
      block->statement = parser_continue_statement();
      break;
    case token::TokenType::key_return:
      block->statement = parser_return_statement();
      break;
    case token::TokenType::key_goto:
      block->statement = parser_goto_statement();
      break;
    case token::TokenType::r_big_par:
      return nullptr;

    // unary operator
    case token::TokenType::self_plus:
    case token::TokenType::self_minus:
    case token::TokenType::times:
    case token::TokenType::bit_and:
    case token::TokenType::plus:
    case token::TokenType::minus:
    case token::TokenType::bit_not:
    case token::TokenType::log_not: {
      block->statement = parser_expression();
      m_scanner.eatCurrentToken(token::TokenType::semicolon);
      break;
    }
    default:
      ERROR_UNEXPECT_TOKEN_1arg(m_scanner.getCurrentToken());
  }

  return block;
}

/**
 *
 * @return
 */
ast::CaseLabel *ParserImpl::parser_case_label()
{
  /*
   * (1) case_label -> case identify :
   */
  auto *caseLabel = new ast::CaseLabel;

  m_scanner.eatCurrentToken(token::TokenType::key_case);

  caseLabel->identifier = parser_identifier();

  m_scanner.eatCurrentToken(token::TokenType::colon);

  return caseLabel;
}

/**
 *
 * @return
 */
ast::DefaultLabel *ParserImpl::parser_default_label()
{
  /*
   * (1) default
   */
  auto *defaultLabel = new ast::DefaultLabel;

  m_scanner.eatCurrentToken(token::TokenType::key_default);

  return defaultLabel;
}

/**
 *
 * @return
 */

ast::IfStatement *ParserImpl::parser_if_statement()
{
  /*
   * if_statement -> if ( assignment_expression ) statement
   *               | if ( assignment_expression ) statement else statement
   */

  auto *ifStatement = new ast::IfStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_if);

  m_scanner.eatCurrentToken(token::TokenType::l_par);
  ifStatement->assignExpression = parser_assignment_expression();
  m_scanner.eatCurrentToken(token::TokenType::r_par);

  ifStatement->ifBody = parser_statement();

  if (m_scanner.getCurrentTokenType() == token::TokenType::key_else)
  {
    m_scanner.eatCurrentToken(token::TokenType::key_else);
    ifStatement->elseBody = parser_statement();
  }

  return ifStatement;
}

/**
 *
 * @return
 */
ast::WhileStatement *ParserImpl::parser_while_statement()
{
  auto *whileStatement = new ast::WhileStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_while);
  m_scanner.eatCurrentToken(token::TokenType::l_par);
  whileStatement->assignmentExpression = parser_assignment_expression();
  m_scanner.eatCurrentToken(token::TokenType::r_par);

  whileStatement->body = parser_compound_statement();

  return whileStatement;
}

/**
 *
 * @return
 */
ast::SwitchStatement *ParserImpl::parser_switch_statement()
{
  auto *switchStatement = new ast::SwitchStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_switch);

  m_scanner.eatCurrentToken(token::TokenType::l_par);
  switchStatement->assignExpression = parser_assignment_expression();
  m_scanner.eatCurrentToken(token::TokenType::r_par);

  switchStatement->compoundStatement = parser_compound_statement();

  return switchStatement;
}

/**
 *
 * @return
 */
ast::ForStatement *ParserImpl::parser_for_statement()
{
  auto *forStatement = new ast::ForStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_for);

  {
    m_scanner.eatCurrentToken(token::TokenType::l_par);

    forStatement->declaration                 = parser_declaration_or_definition();
    forStatement->conditionalAssignExpression = parser_assignment_expression();
    m_scanner.eatCurrentToken(token::TokenType::semicolon);
    forStatement->changeAssignExpression = parser_assignment_expression();

    m_scanner.eatCurrentToken(token::TokenType::r_par);
  }

  forStatement->compoundStatement = parser_compound_statement();

  return forStatement;
}

/**
 *
 * @return
 */
ast::DoWhileStatement *ParserImpl::parser_do_while_statement()
{
  auto *doWhileStatement = new ast::DoWhileStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_do);
  doWhileStatement->compoundStatement = parser_compound_statement();
  m_scanner.eatCurrentToken(token::TokenType::key_while);

  m_scanner.eatCurrentToken(token::TokenType::l_par);
  doWhileStatement->assignExpression = parser_assignment_expression();
  m_scanner.eatCurrentToken(token::TokenType::r_par);
  m_scanner.eatCurrentToken(token::TokenType::semicolon);

  return doWhileStatement;
}

/**
 *
 * @return
 */
ast::BreakStatement *ParserImpl::parser_break_statement()
{
  auto *breakStatement = new ast::BreakStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_break);
  m_scanner.eatCurrentToken(token::TokenType::semicolon);

  return breakStatement;
}

/**
 *
 * @return
 */
ast::ContinueStatement *ParserImpl::parser_continue_statement()
{
  auto *continueStatement = new ast::ContinueStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_continue);

  return continueStatement;
}

/**
 *
 * @return
 */
ast::ReturnStatement *ParserImpl::parser_return_statement()
{
  auto *returnStatement = new ast::ReturnStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_return);

  if (m_scanner.getCurrentTokenType() != token::TokenType::semicolon)
  {
    returnStatement->assignmentExpression = parser_assignment_expression();
  }

  m_scanner.eatCurrentToken(token::TokenType::semicolon);

  return returnStatement;
}

/**
 *
 * @return
 */
ast::Label *ParserImpl::parser_label()
{
  auto *label = new ast::Label;

  label->identifier = parser_identifier();
  m_scanner.eatCurrentToken(token::TokenType::colon);

  return label;
}

/**
 *
 * @return
 */
ast::Expression *ParserImpl::parser_expression()
{
  /*
   * expression -> assignment_expression ;
   *             | expression , assignment_expression ;
   */
  auto *expression = new ast::Expression;

  expression->push_back(parser_assignment_expression());

  while (m_scanner.getCurrentTokenType() == token::TokenType::comma)
  {
    m_scanner.eatCurrentToken(token::TokenType::comma);
    expression->push_back(parser_assignment_expression());
  }

  return expression;
}

/**
 *
 * @return
 */
ast::AssignmentExpression *ParserImpl::parser_assignment_expression()
{
  /*
   * assignment_expression
   * (1) -> unary_expression assignment_operator assignment_expression
   * (2)  | unary_expression
   *      | conditional_expression
   * (3)    -> binary_expression
   * (4)     | binary_expression ? expression : conditional_expression
   */

  auto *assignExpression = new ast::AssignmentExpression;

  auto unaryOrBinaryExpression = parser_unary_or_binary_expression();

  switch (m_scanner.getCurrentTokenType())
  {
    // (1)
    // left is a unary expression
    case token::TokenType::assign:
    case token::TokenType::plus_agn:
    case token::TokenType::minus_agn:
    case token::TokenType::div_agn:
    case token::TokenType::mod_agn:
    case token::TokenType::times_agn:
    case token::TokenType::r_shift_agn:
    case token::TokenType::l_shift_agn:
    case token::TokenType::bit_and_agn:
    case token::TokenType::bit_or_agn:
    case token::TokenType::bit_xor_agn: {
      if (std::get_if<ast::UnaryExpression *>(&unaryOrBinaryExpression) != nullptr)
      {
        auto *unaryExpression = std::get<ast::UnaryExpression *>(unaryOrBinaryExpression);

        assignExpression->unaryExpression = unaryExpression;
        assignExpression->assignmentType  = m_scanner.getCurrentTokenType();
        m_scanner.nextToken();
        assignExpression->nextAssignmentExpression = parser_assignment_expression();
      }
      else
      {
        // can't assign to a binary expression
        TODO
      }
      break;
    }

    // (4)
    // token is ?, a conditional expression
    case token::TokenType::question_mark: {
      m_scanner.eatCurrentToken(token::TokenType::question_mark);
      auto conditionalExpression = new ast::ConditionalExpression;

      assignExpression->conditionalExpression = conditionalExpression;

      conditionalExpression->unaryOrBinaryExpression = unaryOrBinaryExpression;

      m_scanner.eatCurrentToken(token::TokenType::question_mark);
      conditionalExpression->expression = parser_expression();

      m_scanner.eatCurrentToken(token::TokenType::colon);
      conditionalExpression->conditionalExpression = parser_conditional_expression();
      break;
    }

      // (2) (3)
      // end of expression , ; ) ] }
    case token::TokenType::r_par:
    case token::TokenType::r_mid_par:
    case token::TokenType::r_big_par:
    case token::TokenType::semicolon:
    case token::TokenType::comma: {
      if (std::get_if<ast::UnaryExpression *>(&unaryOrBinaryExpression) != nullptr)
      {
        assignExpression->unaryExpression
          = std::get<ast::UnaryExpression *>(unaryOrBinaryExpression);
      }
      else
      {
        assignExpression->binaryExpression
          = std::get<ast::BinaryExpression *>(unaryOrBinaryExpression);
      }
      break;
    }

    default:
      ERROR_UNEXPECT_TOKEN_1arg(m_scanner.getCurrentToken());
  }

  return assignExpression;
}

/**
 *
 * @return
 */
ast::BinaryExpression *ParserImpl::parser_binary_expression()
{
  TODO;
}

/**
 *
 * @return
 */
ast::UnaryExpression *ParserImpl::parser_unary_expression()
{
  /*
   * unary_expression
   * -> postfix_expression
   *  | unary_operator unary_expression
   *  | ( declaration_declarator ) unary_expression
   *  | sizeof unary_expression
   *  | sizeof ( declaration_declarator )
   */


  // assume unary expression is a binary expression in brackets
  // don't create unary expression node
  // so idx_root don't need to initialize
  // idx_root can be an idx of binary expression or unary expression

  auto *unaryExpression = new ast::UnaryExpression;

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::identify:
    case token::TokenType::literal_string:
    case token::TokenType::literal_char:
    case token::TokenType::literal_positive_float:
    case token::TokenType::literal_negative_float:
    case token::TokenType::literal_positive_integer:
    case token::TokenType::literal_negative_integer: {
      unaryExpression->postfixExpression = parser_postfix_expression();
      break;
    }

    case token::TokenType::self_plus:
    case token::TokenType::self_minus:
    case token::TokenType::bit_and:
    case token::TokenType::times:
    case token::TokenType::bit_not:
    case token::TokenType::log_not: {
      unaryExpression->unaryOperator = m_scanner.getCurrentTokenType();
      m_scanner.nextToken();
      unaryExpression->unaryExpression = parser_unary_expression();
      break;
    }

      // cast or (expression)
    case token::TokenType::l_par: {
      switch (m_scanner.getCurrentTokenType())
      {
        // cast
        case token::TokenType::key__Bool:
        case token::TokenType::key__Complex:
        case token::TokenType::key__Imaginary:
        case token::TokenType::key_int:
        case token::TokenType::key_long:
        case token::TokenType::key_short:
        case token::TokenType::key_float:
        case token::TokenType::key_double:
        case token::TokenType::key_unsigned:
        case token::TokenType::key_signed: {
          m_scanner.eatCurrentToken(token::TokenType::l_par);
          unaryExpression->declarationDeclarator = parser_declaration_specifiers();
          m_scanner.eatCurrentToken(token::TokenType::r_par);

          unaryExpression->unaryExpression = parser_unary_expression();
        }
          // ( expression )
        default: {
          unaryExpression->postfixExpression = parser_postfix_expression();
          break;
        }
      }
      break;
    }

    case token::TokenType::key_sizeof: {
      unaryExpression->isSizeof = true;
      m_scanner.eatCurrentToken(token::TokenType::key_sizeof);
      if (m_scanner.getCurrentTokenType() == token::TokenType::l_par)
      {
        m_scanner.eatCurrentToken(token::TokenType::l_par);
        unaryExpression->declarationDeclarator = parser_declaration_specifiers();
      }
      else
      {
        unaryExpression->unaryExpression = parser_unary_expression();
      }
      break;
    }

    case token::TokenType::assign:
    case token::TokenType::plus_agn:
    case token::TokenType::minus_agn:
    case token::TokenType::times_agn:
    case token::TokenType::div_agn:
    case token::TokenType::mod_agn:
    case token::TokenType::r_shift_agn:
    case token::TokenType::l_shift_agn:
    case token::TokenType::bit_and_agn:
    case token::TokenType::bit_or_agn:
    case token::TokenType::bit_xor_agn:
      break;
    default:
      ERROR_UNEXPECT_TOKEN_1arg(m_scanner.getCurrentToken());
  }

  return unaryExpression;
}

/**
 *
 * @return
 */
ast::ConditionalExpression *ParserImpl::parser_conditional_expression()
{
  TODO return nullptr;
}

/**
 *
 * @return
 */
std::variant<ast::UnaryExpression *, ast::BinaryExpression *>
ParserImpl::parser_unary_or_binary_expression()
{
  return parser_priority_binary_expression(m_start_priority);
}

/**
 *
 * @param priority
 * @return
 */
std::variant<ast::UnaryExpression *, ast::BinaryExpression *>
ParserImpl::parser_priority_binary_expression(int priority)
{
  if (priority == m_end_priority)
  {
    return parser_unary_expression();
  }

  // note
  // left
  auto leftExpression = parser_priority_binary_expression(priority - 1);
  auto rootExpression = leftExpression;

  // creat node
  while (operator_priority(m_scanner.getCurrentTokenType()) == priority)
  {
    auto *binaryExpression = new ast::BinaryExpression;
    rootExpression         = binaryExpression;

    binaryExpression->binaryOperator = m_scanner.getCurrentTokenType();

    //[1] right combine operators || &&
    if (m_scanner.getCurrentTokenType() == token::TokenType::log_or || m_scanner.getCurrentTokenType() == token::TokenType::log_and)
    {
      m_scanner.nextToken();  // eat operator

      binaryExpression->leftExpr  = leftExpression;
      binaryExpression->rightExpr = parser_priority_binary_expression(priority);
      break;
    }
    //[2] left combine operators except || &&
    else
    {
      m_scanner.nextToken();  // eat operator

      binaryExpression->leftExpr  = leftExpression;
      binaryExpression->rightExpr = parser_priority_binary_expression(priority - 1);

      leftExpression = binaryExpression;
    }
  }
  // not creat node

  return rootExpression;
}


/**
 *
 * @param token_type_
 * @return
 */
int ParserImpl::operator_priority(token::TokenType token_type_)
{
  switch (token_type_)
  {
    // end of an expression
    // return -1
    case token::TokenType::semicolon:
    case token::TokenType::assign:
    case token::TokenType::plus_agn:
    case token::TokenType::minus_agn:
    case token::TokenType::times_agn:
    case token::TokenType::div_agn:
    case token::TokenType::mod_agn:
    case token::TokenType::r_shift_agn:
    case token::TokenType::l_shift_agn:
    case token::TokenType::bit_and_agn:
    case token::TokenType::bit_or_agn:
    case token::TokenType::bit_xor_agn:
    case token::TokenType::r_par:
    case token::TokenType::r_mid_par:
    case token::TokenType::r_big_par:
    case token::TokenType::comma:
    case token::TokenType::question_mark:
    case token::TokenType::colon:
      return -1;
    case token::TokenType::log_or:
      return 13;
    case token::TokenType::log_and:
      return 12;
    case token::TokenType::bit_or:
      return 11;
    case token::TokenType::bit_xor:
      return 10;
    case token::TokenType::bit_and:
      return 9;
    case token::TokenType::equ:
    case token::TokenType::not_equ:
      return 8;
    case token::TokenType::less:
    case token::TokenType::less_equ:
    case token::TokenType::great:
    case token::TokenType::great_equ:
      return 7;
    case token::TokenType::l_shift:
    case token::TokenType::r_shift:
      return 6;
    case token::TokenType::plus:
    case token::TokenType::minus:
      return 5;
    case token::TokenType::div:
    case token::TokenType::mod:
    case token::TokenType::times:
      return 4;
    default:
      TODO
  }
}

/**
 *
 * @return
 */
ast::PostfixExpression *ParserImpl::parser_postfix_expression()
{
  /*
   * postfix-expression -> primary-expression
   *                     | primary_expression postfix_operator_list
   */

  auto *postfixExpression = new ast::PostfixExpression;

  postfixExpression->primaryExpression   = parser_primary_expression();
  postfixExpression->postfixOperatorList = parser_postfix_operator_list();

  return postfixExpression;
}

/**
 *
 * @return
 */
ast::PrimaryExpression *ParserImpl::parser_primary_expression()
{
  /*
(1) primary-expression -> identifier
(2)                     | constant
(3)                     | string-literal
(4)                     | ( expression )
   */
  auto *primaryExpression = new ast::PrimaryExpression;

  switch (m_scanner.getCurrentTokenType())
  {
    // (1)
    case token::TokenType::identify: {
      primaryExpression->identifier = parser_identifier();
      break;
    }
    case token::TokenType::literal_char: {
      primaryExpression->literalChar = parser_literal_char();
      break;
    }
    // (3)
    case token::TokenType::literal_string: {
      primaryExpression->literalString = parser_literal_string();
      break;
    }
    // (2)
    case token::TokenType::literal_positive_float:
    case token::TokenType::literal_negative_float: {
      primaryExpression->literalFloat = parser_literal_float();
      break;
    }
    case token::TokenType::literal_positive_integer:
    case token::TokenType::literal_negative_integer: {
      primaryExpression->literalInteger = parser_literal_integer();
      break;
    }
      // (4)
    case token::TokenType::l_par: {
      m_scanner.eatCurrentToken(token::TokenType::l_par);
      primaryExpression->expression = parser_expression();
      m_scanner.eatCurrentToken(token::TokenType::r_par);
      break;
    }
    default:
      ERROR_UNEXPECT_TOKEN_1arg(m_scanner.getCurrentToken());
  }

  return primaryExpression;
}

/**
 *
 * @return
 */
ast::PostfixOperatorList *ParserImpl::parser_postfix_operator_list()
{
  auto *postfixOperatorList = new ast::PostfixOperatorList;

  bool isContinue = true;
  while (isContinue)
  {
    switch (m_scanner.getCurrentTokenType())
    {
      case token::TokenType::l_mid_par:
      case token::TokenType::l_par:
      case token::TokenType::arrow:
      case token::TokenType::self_plus:
      case token::TokenType::self_minus:
      case token::TokenType::dot: {
        postfixOperatorList->push_back(parser_postfix_operator());
        break;
      }

      default:
        isContinue = false;
    }
  }

  return postfixOperatorList;
}

/**
 *
 * @return
 */
ast::PostfixOperator *ParserImpl::parser_postfix_operator()
{
  /*
   * postfix-operator -> [ expression ]
   *                   | ( assignment_expression_list )
   *                   | . identify
   *                   | -> identify
   *                   | ++
   *                   | --
   */

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::l_mid_par:
      return parser_array_postfix_operator();
    case token::TokenType::l_par:
      return parser_function_postfix_operator();
    case token::TokenType::arrow:
      return parser_arrow_postfix_operator();
    case token::TokenType::self_plus:
      return parser_self_plus_postfix_operator();
    case token::TokenType::self_minus:
      return parser_self_minus_postfix_operator();
    case token::TokenType::dot:
      return parser_member_postfix_operator();
    default:
      TODO
  }
}

/**
 *
 * @return
 */
ast::LiteralString *ParserImpl::parser_literal_string()
{
  auto *literalString  = new ast::LiteralString;
  literalString->value = m_scanner.getCurrentTokenStr();

  m_scanner.eatCurrentToken(token::TokenType::literal_string);

  return literalString;
}

/**
 *
 * @return
 */
ast::LiteralInteger *ParserImpl::parser_literal_integer()
{
  auto *literalInteger = new ast::LiteralInteger;

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::literal_positive_integer: {
      literalInteger->value      = m_scanner.getCurrentTokenStr();
      literalInteger->isNegative = false;
      m_scanner.eatCurrentToken(token::TokenType::literal_positive_integer);
      break;
    }
    case token::TokenType::literal_negative_integer: {
      literalInteger->value      = m_scanner.getCurrentTokenStr();
      literalInteger->isNegative = true;
      m_scanner.eatCurrentToken(token::TokenType::literal_negative_integer);
      break;
    }
    default:
      TODO
  }

  return literalInteger;
}

/**
 *
 * @return
 */
ast::LiteralFloat *ParserImpl::parser_literal_float()
{
  auto *literalFloat = new ast::LiteralFloat;

  switch (m_scanner.getCurrentTokenType())
  {
    case token::TokenType::literal_positive_float: {
      literalFloat->value      = m_scanner.getCurrentTokenStr();
      literalFloat->isNegative = false;
      m_scanner.eatCurrentToken(token::TokenType::literal_positive_float);
      break;
    }
    case token::TokenType::literal_negative_float: {
      literalFloat->value      = m_scanner.getCurrentTokenStr();
      literalFloat->isNegative = true;
      m_scanner.eatCurrentToken(token::TokenType::literal_negative_float);
      break;
    }
    default:
      TODO
  }

  return literalFloat;
}

/**
 *
 * @return
 */
ast::ArrayPostfixOperator *ParserImpl::parser_array_postfix_operator()
{
  auto *arrayPostfixOperator = new ast::ArrayPostfixOperator;

  m_scanner.eatCurrentToken(token::TokenType::l_mid_par);
  arrayPostfixOperator->expression = parser_expression();
  m_scanner.eatCurrentToken(token::TokenType::r_mid_par);

  return arrayPostfixOperator;
}

/**
 *
 * @return
 */
ast::FunctionPostfixOperator *ParserImpl::parser_function_postfix_operator()
{
  auto *functionPostfixOperator = new ast::FunctionPostfixOperator;

  m_scanner.eatCurrentToken(token::TokenType::l_par);
  functionPostfixOperator->assignmentExpressionList = parser_assignment_expression_list();
  m_scanner.eatCurrentToken(token::TokenType::r_par);

  return functionPostfixOperator;
}

/**
 *
 * @return
 */
ast::MemberPostfixOperator *ParserImpl::parser_member_postfix_operator()
{
  auto *memberPostfixOperator = new ast::MemberPostfixOperator;

  m_scanner.eatCurrentToken(token::TokenType::dot);
  memberPostfixOperator->identifier = parser_identifier();

  return memberPostfixOperator;
}

/**
 *
 * @return
 */
ast::ArrowPostfixOperator *ParserImpl::parser_arrow_postfix_operator()
{
  auto *arrowPostfixOperator = new ast::ArrowPostfixOperator;

  m_scanner.eatCurrentToken(token::TokenType::arrow);
  arrowPostfixOperator->identifier = parser_identifier();

  return arrowPostfixOperator;
}

/**
 *
 * @return
 */
ast::SelfPlusPostfixOperator *ParserImpl::parser_self_plus_postfix_operator()
{
  m_scanner.eatCurrentToken(token::TokenType::self_plus);
  return new ast::SelfPlusPostfixOperator;
}

/**
 *
 * @return
 */
ast::SelfMinusPostfixOperator *ParserImpl::parser_self_minus_postfix_operator()
{
  m_scanner.eatCurrentToken(token::TokenType::self_minus);
  return new ast::SelfMinusPostfixOperator;
}

/**
 *
 * @return
 */
ast::AssignmentExpressionList *ParserImpl::parser_assignment_expression_list()
{
  auto *assignmentExpressionList = new ast::AssignmentExpressionList;

  while (m_scanner.getCurrentTokenType() != token::TokenType::r_par)
  {
    assignmentExpressionList->push_back(parser_assignment_expression());
    if (m_scanner.getCurrentTokenType() == token::TokenType::comma)
    {
      m_scanner.eatCurrentToken(token::TokenType::comma);
    }
  }

  return assignmentExpressionList;
}

/**
 *
 * @return
 */
ast::GotoStatement *ParserImpl::parser_goto_statement()
{
  auto *gotoStatement = new ast::GotoStatement;

  m_scanner.eatCurrentToken(token::TokenType::key_goto);
  gotoStatement->identifier = parser_identifier();
  m_scanner.eatCurrentToken(token::TokenType::semicolon);

  return gotoStatement;
}

/**
 *
 * @return
 */
ast::StructOrUnionSpecifier *ParserImpl::parser_struct_or_union_specifier()
{
  /*
   * struct_or_union_specifier
   * -> struct_or_union identifier { struct_declaration_list }
   *  | struct_or_union identifier
   *    -> struct
   *     | union
   */
  auto *structOrUnionSpecifier = new ast::StructOrUnionSpecifier;

  // eat struct or union
  m_scanner.nextToken();

  structOrUnionSpecifier->identifier = parser_identifier();

  if (m_scanner.getCurrentTokenType() == token::TokenType::l_big_par)
  {
    m_scanner.eatCurrentToken(token::TokenType::l_big_par);
    structOrUnionSpecifier->structDeclarationList = parser_struct_declaration_list();
    m_scanner.eatCurrentToken(token::TokenType::r_big_par);
  }

  return structOrUnionSpecifier;
}

/**
 *
 * @return
 */
ast::StructDeclarationList *ParserImpl::parser_struct_declaration_list()
{
  /*
   * struct_declaration_list
   * -> struct_declaration struct_declaration_list
   */
  auto *structDeclarationList = new ast::StructDeclarationList;

  while (m_scanner.getCurrentTokenType() != token::TokenType::r_big_par)
  {
    structDeclarationList->push_back(parser_struct_declaration());
  }

  return structDeclarationList;
}

/**
 *
 * @return
 */
ast::StructDeclaration *ParserImpl::parser_struct_declaration()
{
  /*
   * struct_declaration
   * -> specifier_qualifier_list struct_declarator_list
   */
  auto *structDeclaration = new ast::StructDeclaration;

  structDeclaration->specifierQualifierList = parser_specifier_qualifier_list();
  structDeclaration->structDeclaratorList   = parser_struct_declarator_list();

  m_scanner.eatCurrentToken(token::TokenType::semicolon);

  return structDeclaration;
}

/**
 *
 * @return
 */
ast::SpecifierQualifierList *ParserImpl::parser_specifier_qualifier_list()
{
  /* specifier_qualifier_list
   * -> type_specifier specifier_qualifier_list
   *  | type_specifier
   *  | type_qualifier specifier_qualifier_list
   *  | type_qualifier
   */
  auto *specifierQualifierList = new ast::SpecifierQualifierList;

  bool isContinue = true;

  while (isContinue)
  {
    switch (m_scanner.getCurrentTokenType())
    {
      //
      // type qualifier
      //
      case token::TokenType::key_const:
        specifierQualifierList->typeQualifierList.push_back(ast::TypeQualifier::_const_);
        break;
      case token::TokenType::key_volatile:
        specifierQualifierList->typeQualifierList.push_back(ast::TypeQualifier::_volatile_
        );
        break;
      case token::TokenType::key_restrict:
        specifierQualifierList->typeQualifierList.push_back(ast::TypeQualifier::_restrict_
        );
        break;
      //
      // type specifier
      //
      case token::TokenType::key_void: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_void_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_char: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_char_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_short: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_short_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_int: {
        if (auto &typeSpecifier = specifierQualifierList->typeSpecifier;
            typeSpecifier != ast::TypeSpecifier::_long_
            || typeSpecifier != ast::TypeSpecifier::_long_long_)
        {
          typeSpecifier = ast::TypeSpecifier::_int_;
        }
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_long: {
        auto &typeSpecifier = specifierQualifierList->typeSpecifier;
        if (typeSpecifier == ast::TypeSpecifier::_int_)
        {
          typeSpecifier = ast::TypeSpecifier::_long_;
        }
        else if (typeSpecifier == ast::TypeSpecifier::_long_)
        {
          typeSpecifier = ast::TypeSpecifier::_long_long_;
        }
        else
        {
          typeSpecifier = ast::TypeSpecifier::_long_;
        }
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_float: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_float_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_double: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_double_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_signed: {
        specifierQualifierList->isSigned = true;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_unsigned: {
        specifierQualifierList->isSigned = false;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key__Bool: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::__Bool_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key__Complex: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::__Complex_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_struct: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_struct_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_union: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_union_;
        m_scanner.nextToken();
        break;
      }
      case token::TokenType::key_enum: {
        specifierQualifierList->typeSpecifier = ast::TypeSpecifier::_enum_;
        m_scanner.nextToken();
        break;
      }
      default:
        isContinue = false;
        break;
    }
  }

  return specifierQualifierList;
}

/**
 *
 * @return
 */
ast::StructDeclaratorList *ParserImpl::parser_struct_declarator_list()
{
  /*
   * struct_declarator_list
   * -> struct_declarator , struct_declarator_list
   *  | struct_declarator
   */
  auto *structDeclaratorList = new ast::StructDeclaratorList;
  bool  isContinue           = true;

  do
  {
    structDeclaratorList->push_back(parser_struct_declarator());
    if (m_scanner.getCurrentTokenType() == token::TokenType::comma)
    {
      m_scanner.eatCurrentToken(token::TokenType::comma);
    }
    else
    {
      isContinue = false;
    }
  } while (isContinue);

  return structDeclaratorList;
}

/**
 *
 * @return
 */
ast::StructDeclarator *ParserImpl::parser_struct_declarator()
{
  /*
   * struct_declarator
   * -> declarator
   *  | declarator : constant_expression
   *  | : constant_expression
   */
  auto *structDeclarator = new ast::StructDeclarator;

  if (m_scanner.getCurrentTokenType() != token::TokenType::colon)
  {
    structDeclarator->declarator = parser_declarator();
    if (m_scanner.getCurrentTokenType() == token::TokenType::colon)
    {
      m_scanner.eatCurrentToken(token::TokenType::colon);
      structDeclarator->assignmentExpression = parser_assignment_expression();
    }
  }
  else
  {
    m_scanner.eatCurrentToken(token::TokenType::colon);
    structDeclarator->assignmentExpression = parser_assignment_expression();
  }

  return structDeclarator;
}

ast::Pointer *ParserImpl::parser_pointer()
{
  /*
   * pointer
   * -> * type_qualifier_list
   *  | * type_qualifier_list pointer
   *  | * pointer
   *  | *
   *
   *  type_qualifier_list can be empty, when it is empty
   *  parser_type_qualifier_list() will return nullptr
   */
  ast::Pointer *pointer = nullptr;

  if (m_scanner.getCurrentTokenType() == token::TokenType::times)
  {
    m_scanner.eatCurrentToken(token::TokenType::times);
    pointer                    = new ast::Pointer;
    pointer->typeQualifierList = parser_type_qualifier_list();
    pointer->nextPointer       = parser_pointer();
  }

  return pointer;
}

/**
 *
 * @return
 */
ast::TypeQualifierList *ParserImpl::parser_type_qualifier_list()
{
  /*
   * type_qualifier_list
   * -> type_qualifier type_qualifier_list
   *  | type_qualifier
   */

  ast::TypeQualifierList *typeQualifierList = nullptr;

  bool isContinue = true;
  while (isContinue)
  {
    switch (m_scanner.getCurrentTokenType())
    {
      case token::TokenType::key_const: {
        if (typeQualifierList == nullptr)
        {
          typeQualifierList = new ast::TypeQualifierList;
        }
        typeQualifierList->push_back(ast::TypeQualifier::_const_);
        break;
      }
      case token::TokenType::key_restrict: {
        if (typeQualifierList == nullptr)
        {
          typeQualifierList = new ast::TypeQualifierList;
        }
        typeQualifierList->push_back(ast::TypeQualifier::_restrict_);
        break;
      }
      case token::TokenType::key_volatile: {
        if (typeQualifierList == nullptr)
        {
          typeQualifierList = new ast::TypeQualifierList;
        }
        typeQualifierList->push_back(ast::TypeQualifier::_volatile_);
        break;
      }
      default:
        isContinue = false;
    }
  }

  return typeQualifierList;
}

/**
 *
 * @return
 */
ast::LiteralChar *ParserImpl::parser_literal_char()
{
  auto *literalChar  = new ast::LiteralChar;
  literalChar->value = m_scanner.getCurrentTokenStr()[0];
  m_scanner.eatCurrentToken(token::TokenType::literal_char);
  return literalChar;
}


}  // namespace toy_c::parser
