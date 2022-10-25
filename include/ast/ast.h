#ifndef syntax_TREE_H
#define syntax_TREE_H
#pragma once
#include "ast_node.h"
#include "declataror.h"
#include <fstream>
#include <string>
#include <vector>


namespace ast
{
std::size_t const sizeof_syntax_tree_init = 100000;


// TODO
// no need to reserve tree
// delete code of reverse syntax tree
class Tree
{
private:
  // DROP
  bool reserve_tree     = false;
  bool print_trans_tree = false;

  // DROP
  enum type_decl_defi
  {
    func_decl,
    func_defi,
    struct_decl,
    struct_defi,
    array_decl,
    var_decl,
    enum_defi,
  };

  std::size_t now_idx;

  ast::idx last_root_ptr = ast::null;

  std::string file_buffer;

  std::vector<ast::node>          tree_body;
  std::vector<ast::constant_node> constant_node_tree;

  ast::idx idx_now_declaration_declarator = ast::null;
  ast::idx idx_now_compound_statement     = ast::null;

  void dfs_print_tree(ast::idx ptr);
  void print_json_key_value(std::string key, std::string value);
  void print_json_key(std::string key);
  void print_json_value(std::string value);
  void print_json_class_head(std::string value);
  void print_json_constant(ast::idx idx_constant);
  void print_json_class_end();
  void trans_declaration_or_definition(ast::idx idx);
  void trans_each_initial_declarator(ast::idx idx);

  type_decl_defi which_type(ast::idx idx);

  void trans_to_function(ast::idx idx);
  void trans_to_arrary_definition(ast::idx idx);
  void trans_to_struct_definition(ast::idx idx);
  void trans_to_struct_declaration(ast::idx idx);
  void trans_to_basic_type_declaration(ast::idx idx);
  void trans_to_enum_definition(ast::idx idx);

public:
  Tree();
  ~Tree();
  ast::Tree &operator=(const ast::Tree &&Tree);

  ast::idx insert(ast::node_type node_type);  // DROP
  ast::idx creat_node(ast::node_type node_type);
  ast::idx create_constant_node(ast::node_type node_type, std::string value);

  void connect(ast::idx ptr);
  void earse(ast::idx idx);
  void print_tree();
  void to_json();
  void trans_tree();

  ast::node          &operator[](ast::idx ptr);
  ast::constant_node &get_constant(ast::idx idx_constant);
};

}  // namespace ast

#endif
