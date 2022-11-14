#ifndef SPEC_TREE_H
#define SPEC_TREE_H

#include "ast.h"
#include "spec_symbol_table.h"
#include "spec_tree_node.h"
#include <vector>

namespace spt
{

class Tree
{
private:
  Block *ptr_root_tree_body = nullptr;

  ast::Tree         ast;
  spec_symbol_table symbol_table;

  // DROP
  std::vector<spt::spec_tree_node> tree_body;

  ast::idx now_compound_statement;

  std::string get_identifier_name(ast::idx idx_declarator);

  Block *build_multi_declaration_or_definition(ast::idx idx_declaration_or_definition);

  spt::Statement *build_declaration_or_definition(
    ast::idx idx_declaration_declarator,
    ast::idx idx_initial_declarator
  );

  std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
  build_arguments_type_list(ast::idx idx_arguments_type_list);

  std::tuple<spt::Type *, std::string>
  build_argument_declaration(ast::idx idx_argument_declaration);

  std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
  build_struct_element_type_list(ast::idx idx_compound_statement);
  Type *
  build_pointer(spt::Type *ptr_type_declaration_declarator, ast::idx idx_declarator);
  Type      *build_declaration_declarator(ast::idx idx_declaration_declarator);
  ArrayType *build_array(spt::Type *ptr_unit_type, ast::idx idx_array_declarator);
  Block     *build_compound_statement(ast::idx idx_compound_statement);

  Expr     *build_expression(ast::idx idx_expression);
  Expr     *build_assign_expression(ast::idx idx_assignment_expression);
  Expr     *build_conditional_expression(ast::idx idx_conditional_expression);
  Expr     *build_unary_expression(ast::idx idx_unary_expression);
  Expr     *build_binary_expression(ast::idx idx_binary_expression);
  Expr     *build_postfix_expression(ast::idx idx_postfix_expression);
  Expr     *build_conversion(ast::idx idx_unary_expression);
  Constant *build_constant(ast::idx idx_constant);

  PostfixOperator *build_postfix_operator(ast::idx idx_postfix_operator);

  std::vector<Expr *> build_argument_list(ast::idx idx_expression_list);

  // size of
  Expr *build_compute_unary_expression_size(ast::idx unary_expression);
  Expr *build_compute_declarator_size(ast::idx idx_declaration_declarator);

  // TODO
  Statement *build_statement(ast::idx idx_statement);

  Statement *build_if_statement(ast::idx idx_statement);
  Statement *build_switch_statement(ast::idx idx_statement);
  Statement *build_return_statement(ast::idx idx_statement);
  Statement *build_while_statement(ast::idx idx_statement);
  Statement *build_do_while_statement(ast::idx idx_statement);
  Statement *build_for_statement(ast::idx idx_statement);

  std::uint64_t constant_node_to_uint64(ast::idx idx_constant);

  Type *build_type(ast::idx idx_declaration_declarator, ast::idx idx_declarator);


public:
  void print_spec_tree();
  Tree();
  ~Tree() = default;
};

}  // namespace spt

#endif