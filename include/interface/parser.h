#ifndef PARSER_H
#define PARSER_H

#pragma once
#include "asm_code.h"
#include "ast.h"
#include "global_var.h"
#include "scanning.h"
#include "syntax_tree.h"
#include "token.h"
#include <fstream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <tuple>

namespace toy_c {
class parser {
private:
  // yes or no
  bool really_output_asm_code = false;

  // std::vector<statement> code;
  // std::size_tlabel = 0;

  // TODO
  // symbol_table table = symbol_table(output_file_path,
  // really_output_asm_code);
  // symbol_table* table;
  toy_c::scanning    scan = toy_c::scanning(input_file_path);
  toy_c::syntax_tree tree;
  // std::unique_ptr<asm_code> asm_file  =
  // std::make_unique<asm_code>(asm_code(fil)) asm_code* asm_file = nullptr;
  //
  bool is_func   = false;
  bool is_struct = false;

  //临时变量的个数,类似会累加的闭包,parser::get_var_time()
  // std::size_t var_time = 0;

  // void parser_if_statement ();
  // void parser_while_statement();
  // std::string parser_return_statement();

  //解析----没有完工
  // void parser_statement();
  // void parser_function_define_or_declare(std::string func_name, token
  // func_return_type); void parser_expression_(std::string l_value);
  // TODO这两个函数
  // std::string parser_expression();
  // std::string parser_unit(std::string name_array);
  // void parser_primary_expression();
  // void parser_lvalue();
  // TODO add support for pointer and array declare;
  // TODO now don't support pointer and array
  // void parser_declare();
  // void parser_expression_unit();
  // std::string parser_func_call(std::string name_func);
  // std::string get_var_time();
  // std::string get_label();

  //前缀表达式转后缀表达式
  //本应该写在函数中，在类中减少内存反复分配
  // std::vector<std::tuple<token, std::string>> parser_pre_to_pos();
  // DROP
  // void parser_declare_or_function_define_or_declare();
  int const start_priority = 13;
  int const end_priority   = 3;

  // NEW
  // declare
  ast::idx parser_declaration_or_definition();
  ast::idx parser_declaration_declarator();
  ast::idx parser_initial_declarator_list();
  ast::idx parser_initial_declarator();
  ast::idx parser_declarator();
  ast::idx parser_direct_declarator();
  ast::idx parser_arguments_type_list();
  ast::idx parser_arguments_list();
  ast::idx parser_arguments_declaration();
  ast::idx parser_array_declarator();
  ast::idx parser_initializer();
  ast::idx parser_initializer_list();
  // NEW
  // expression
  ast::idx parser_priority_binary_expression(int priority);
  ast::idx parser_expression();
  ast::idx parser_assignment_expression(ast::idx last_assign);
  ast::idx parser_assignment_expression_list();
  ast::idx parser_conditional_expression();
  ast::idx parser_binary_expression();
  ast::idx parser_unary_expression();
  ast::idx parser_postfix_expression();
  ast::idx parser_postfix_operator();
  ast::idx parser_primary_expression();
  ast::idx parser_const_expression();
  ast::idx parser_case_label();
  ast::idx parser_default_label();
  // NEW
  // statement
  ast::idx parser_compound_statement();
  ast::idx parser_block_list();
  ast::idx parser_block();
  ast::idx parser_mark_statement();
  ast::idx parser_statement();
  ast::idx parser_mark();
  ast::idx parser_constant();
  ast::idx parser_not_mark_statement();
  ast::idx parser_if_statement();
  ast::idx parser_switch_statement();
  ast::idx parser_while_statement();
  ast::idx parser_do_while_statement();
  ast::idx parser_for_statement();
  ast::idx parser_break_statement();
  ast::idx parser_goto_statement();
  ast::idx parser_continue_statement();
  ast::idx parser_return_statement();

  ast::idx parser_identifier();

  int operator_priority(token t);

public:
  explicit parser();
  ~parser();
  parser(parser const &)       = delete;
  parser(parser &&)            = delete;
  parser &operator=(parser &)  = delete;
  parser &operator=(parser &&) = delete;

  void                print_mid_code();
  void                print_asm_code_code();
  void                print_syntax_tree();
  void                move_syntax_tree();
  toy_c::syntax_tree &get_syntax_tree();
};

}  // namespace toy_c

#endif