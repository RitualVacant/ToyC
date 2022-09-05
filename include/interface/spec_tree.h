#include "spec_tree_node.h"
#include "syntax_tree.h"


namespace toy_c
{

class spec_tree
{
private:
  toy_c::syntax_tree tree;

  ast::idx now_compound_statement;

  std::string get_identifier_name(ast::idx idx_declarator);

  void trans_mult_declaration_or_definition();
  void trans_declaration_or_definition();
  void trans_declaration_or_definition(
    ast::idx idx_declaration_declarator,
    ast::idx idx_initial_declarator
  );

  std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
  trans_arguments_type_list(ast::idx idx_arguments_type_list);

  std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
  trans_struct_element_type_list(ast::idx idx_compound_statement);

  spt::Type *
  trans_pointer(spt::Type *ptr_type_declaration_declarator, ast::idx idx_declarator);
  spt::Type      *trans_declaration_declarator(ast::idx idx_declaration_declarator);
  spt::ArrayType *trans_array(spt::Type *ptr_unit_type, ast::idx idx_array_declarator);
  spt::Block     *trans_compound_statement(ast::idx idx_compound_statement);
  spt::Expr      *trans_expression(ast::idx idx_expression);

  std::uint64_t constant_node_to_uint64(ast::idx idx_constant);

public:
  spec_tree(toy_c::syntax_tree &syntax_tree);
  ~spec_tree();
};

}  // namespace toy_c