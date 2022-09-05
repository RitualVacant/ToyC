#include "spec_tree_node.h"
#include "syntax_tree.h"

namespace ast
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

  std::tuple<std::vector<ast::Type *>, std::vector<std::string>>
  trans_arguments_type_list(ast::idx idx_arguments_type_list);

  ast::Type *
  trans_pointer(ast::Type *ptr_type_declaration_declarator, ast::idx idx_declarator);
  ast::Type  *trans_declaration_declarator(ast::idx idx_declaration_declarator);
  ast::Type  *trans_array(ast::Type *ptr_unit_type, ast::idx idx_array_declarator);
  ast::Block *trans_compound_statement(ast::idx idx_compound_statement);

  std::uint64_t constant_node_to_uint64(ast::idx idx_constant);

public:
  spec_tree(toy_c::syntax_tree &syntax_tree);
  ~spec_tree();
};

}  // namespace ast