#ifndef BUILD_LLVM_IR_H
#define BUILD_LLVM_IR_H

#pragma once
#include "parser.h"
#include "syntax_tree.h"
#include <vector>


#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include <map>

namespace toy_c {

enum type_of_def_or_dec { is_func_decl, is_array, is_var };

class build_llvm_ir {
private:
  std::unique_ptr<llvm::LLVMContext>   context;
  std::unique_ptr<llvm::Module>        module;
  std::unique_ptr<llvm::IRBuilder<>>   builder;
  toy_c::parser                        parse;
  toy_c::syntax_tree                   tree;
  std::map<std::string, llvm::Value *> func_symbol_table;
  std::map<std::string, llvm::Value *> global_symbol_table;

  ast::idx    now_compound_statement;
  std::size_t label_num = 0;

  llvm::Function   *ptr_now_func;
  llvm::Type       *ptr_now_func_type;
  llvm::BasicBlock *ptr_now_block;

  type_of_def_or_dec which_type(ast::idx);
  bool               is_func_decl(ast::idx);
  bool               is_func(ast::idx);
  bool               is_struct(ast::idx);
  bool               is_union(ast::idx);
  bool               is_var(ast::idx);

  void         insert_func_symbol(std::string name, llvm::Value *ptr_var);
  void         insert_symbol_symbol(std::string name, llvm::Value *ptr_var);
  llvm::Value *find_value(std::string name);

  llvm::Type *build_mult_declaration_or_definition(ast::idx idx_declaration_or_definition
  );
  llvm::Type *
  build_declaration_or_definition(ast::idx idx, llvm::Type *ptr_declaration_declarator);
  llvm::Type *build_declaration_declarator(ast::idx idx);
  llvm::Type *build_type(ast::idx idx_declarator, ast::idx idx_declaration_declarator);
  llvm::Type *
  build_pointer(llvm::Type *ptr_type_declaration_declarator, ast::idx idx_declarator);
  llvm::Type *build_array(llvm::Type *ptr_unit_type, ast::idx idx_array_declarator);
  llvm::Type *
  build_recurs_array(llvm::Type *ptr_unit_type, ast::idx idx_array_declarator);
  llvm::BasicBlock *build_compound_statement(
    ast::idx          idx_compound_statement,
    llvm::BasicBlock *ptr_block = nullptr
  );
  void build_declaration_or_definition(
    ast::idx idx_declaration_declarator,
    ast::idx idx_initial_declarator
  );
  void build_block(ast::idx idx_block);
  void build_variable(ast::idx idx, llvm::Type *ptr_declaration_declarator);
  void
  build_function_or_function_ptr(ast::idx idx, llvm::Type *ptr_declaration_declarator);
  void build_function(ast::idx);
  void build_expression(ast::idx idx_expression);
  void build_return_statement(ast::idx idx_return_statement);
  void build_if_statement(ast::idx idx_if_statement);
  void build_while_statement(ast::idx idx_while_statement);
  void build_switch_statement(ast::idx idx_switch_statement);
  void build_do_while_statement(ast::idx idx_do_while_statement);
  void build_for_statement(ast::idx idx_for_statement);

  llvm::Value *build_assign_expression(
    ast::idx          idx_assign_expression,
    llvm::BasicBlock *ptr_true_block  = nullptr,
    llvm::BasicBlock *ptr_false_block = nullptr,
    bool              is_return_value = true
  );
  llvm::Value *build_binary_expression(
    ast::idx          idx_binary_expression,
    llvm::BasicBlock *ptr_true_block  = nullptr,
    llvm::BasicBlock *ptr_false_block = nullptr,
    bool              is_return_value = true
  );
  llvm::Value *build_unary_expression(ast::idx idx_unary_expression);
  llvm::Value *build_postfix_expression(ast::idx idx_postfix_expression);
  llvm::Value *build_primary_expression(ast::idx idx_primary_expression);
  llvm::Value *build_constant(ast::idx idx_constant);
  llvm::Value *build_unary_operator(ast::idx idx_unary_expression);
  llvm::Value *build_compute_unary_expression_size(ast::idx idx_unary_expression);
  llvm::Value *build_compute_declarator_size(ast::idx idx_declarator);

  // TODO
  // idx type is unknow
  std::uint64_t build_uint64(ast::idx idx_);


  // func for analysis node type and assign type to node
  llvm::Type *analysis_and_assign_type_to_node(
    ast::idx    idx_binary_or_unary_expression,
    llvm::Type *left_value_type,
    llvm::Type *father_node_type = nullptr
  );
  void
  assign_type_to_all_son_node(ast::idx idx_binary_or_unary_expression, llvm::Type *type);


  // DROP
  llvm::Value *build_log_and_chain(ast::idx idx_log_and_operator);
  llvm::Value *build_log_or_chain(ast::idx idx_log_or_operator);

  llvm::SmallVector<llvm::Type *> build_arguments_type_list(ast::idx idx);
  llvm::Type                     *build_argument_declaration(ast::idx idx);


  std::string get_label();

public:
  build_llvm_ir();
  ~build_llvm_ir();
  void output_llvm_ir();
};

}  // namespace toy_c

#endif
