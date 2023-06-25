//
// Created by lzj on 2023.6.7.
//

#ifndef TOYC_PARSER_IMPL_H
#define TOYC_PARSER_IMPL_H

#include "ast/AstNode.h"
#include "ast/AstNodeExpression.h"
#include "parser/ParserImpl.h"
#include "scan/scanner.h"
#include <variant>

namespace toy_c::parser
{

using AstTranslationUnit = std::vector<ast::DeclarationOrDefinition *>;

class ParserImpl
{
public:
  ParserImpl();
  ~ParserImpl() = default;

protected:
  AstTranslationUnit m_translationUnit;

private:
  scan::Scanner m_scanner;

  // new
  ast::TranslationUnit *parser_translation_unit();

  // declaration defines
  ast::DeclarationOrDefinition *parser_declaration_or_definition();
  ast::DeclarationSpecifiers   *parser_declaration_specifiers();
  ast::InitialDeclaratorList   *parser_initial_declarator_list();
  ast::InitialDeclarator       *parser_initial_declarator();
  ast::Declarator              *parser_declarator();
  ast::Pointer                 *parser_pointer();
  ast::Initializer             *parser_initializer();
  ast::DirectDeclarator        *parser_direct_declarator();
  ast::Identifier              *parser_identifier();
  ast::ParameterTypeList       *parser_parameter_type_list();
  ast::ArrayDeclaratorList     *parser_array_declarator_list();
  ast::ArrayDeclarator         *parser_array_declarator();
  ast::ParameterList           *parser_parameter_list();
  ast::IdentifierList          *parser_identifier_list();
  ast::ParameterDeclaration    *parser_parameter_declaration();
  ast::InitializerList         *parser_initializer_list();
  ast::StructOrUnionSpecifier  *parser_struct_or_union_specifier();
  ast::StructDeclarationList   *parser_struct_declaration_list();
  ast::StructDeclaration       *parser_struct_declaration();
  ast::SpecifierQualifierList  *parser_specifier_qualifier_list();
  ast::StructDeclaratorList    *parser_struct_declarator_list();
  ast::StructDeclarator        *parser_struct_declarator();
  ast::TypeQualifierList       *parser_type_qualifier_list();
  ast::Designation             *parser_designation();
  ast::Designator_list         *parser_designator_list();
  ast::Designator              *parser_designator();

  // statement
  ast::Statement         *parser_statement();
  ast::CompoundStatement *parser_compound_statement();
  ast::BlockList         *parser_block_list();
  ast::Block             *parser_block();
  ast::IfStatement       *parser_if_statement();
  ast::WhileStatement    *parser_while_statement();
  ast::SwitchStatement   *parser_switch_statement();
  ast::DoWhileStatement  *parser_do_while_statement();
  ast::ForStatement      *parser_for_statement();
  ast::BreakStatement    *parser_break_statement();
  ast::ContinueStatement *parser_continue_statement();
  ast::ReturnStatement   *parser_return_statement();
  ast::CaseLabel         *parser_case_label();
  ast::DefaultLabel      *parser_default_label();
  ast::Label             *parser_label();
  ast::GotoStatement     *parser_goto_statement();

  // expression
  ast::Expression            *parser_expression();
  ast::AssignmentExpression  *parser_assignment_expression();
  ast::BinaryExpression      *parser_binary_expression();
  ast::UnaryExpression       *parser_unary_expression();
  ast::ConditionalExpression *parser_conditional_expression();
  ast::PostfixExpression     *parser_postfix_expression();
  ast::PrimaryExpression     *parser_primary_expression();
  ast::PostfixOperatorList   *parser_postfix_operator_list();
  ast::PostfixOperator       *parser_postfix_operator();
  ast::LiteralString         *parser_literal_string();
  ast::LiteralInteger        *parser_literal_integer();
  ast::LiteralFloat          *parser_literal_float();
  ast::LiteralChar           *parser_literal_char();

  //
  ast::ArrayPostfixOperator     *parser_array_postfix_operator();
  ast::FunctionPostfixOperator  *parser_function_postfix_operator();
  ast::MemberPostfixOperator    *parser_member_postfix_operator();
  ast::ArrowPostfixOperator     *parser_arrow_postfix_operator();
  ast::SelfPlusPostfixOperator  *parser_self_plus_postfix_operator();
  ast::SelfMinusPostfixOperator *parser_self_minus_postfix_operator();
  ast::AssignmentExpressionList *parser_assignment_expression_list();

  int const m_end_priority   = 3;
  int const m_start_priority = 13;
  std::variant<ast::UnaryExpression *, ast::BinaryExpression *>
  parser_priority_binary_expression(int priority);

  std::variant<ast::UnaryExpression *, ast::BinaryExpression *>
  parser_unary_or_binary_expression();

  static int operator_priority(token::TokenType token_type_);
};

}  // namespace toy_c::parser


#endif  // TOYC_PARSER_IMPL_H
