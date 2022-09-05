#include "spec_tree.h"
#include "ast.h"
#include "inner.h"
#include "spec_tree_node.h"

namespace toy_c
{

/**
 * @brief Construct a new spec tree::spec tree object
 *
 * @param syntax_tree
 */
spec_tree::spec_tree(toy_c::syntax_tree &syntax_tree)
{
  this->tree = std::move(syntax_tree);
}

/**
 * @brief Destroy the spec tree::spec tree object
 *
 */
spec_tree::~spec_tree() {}

/**
 * @brief
 *
 */
void spec_tree::trans_mult_declaration_or_definition()
{
  // the syntax tree 's root node must be 1
  ast::idx idx_declaration_or_definition = 1;

  //
  // idx_declaration_declarator
  //          ||
  //          ||        j
  //          ||       ||
  //          \/       \/
  //  i->    int a, b, c = 3, *d;
  //

  for (ast::idx i = idx_declaration_or_definition,

                idx_declaration_declarator
                = tree[i].value.declaration_or_definition.idx_declaration_declarator;

       i != ast::null;

       i = tree[i].value.declaration_or_definition.idx_next_declaration_or_definition,

                idx_declaration_declarator
                = tree[i].value.declaration_or_definition.idx_declaration_declarator)
  {
    now_compound_statement
      = tree[i].value.declaration_or_definition.idx_compound_statement;

    for (ast::idx j = tree[i].value.declaration_or_definition.idx_initial_declarator;
         j != ast::null; j = tree[j].value.initial_declarator.idx_next_initial_declarator)
    {
      trans_declaration_or_definition(idx_declaration_declarator, j);
    }
  }
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @return spt::Type*
 */
spt::Type *spec_tree::trans_declaration_declarator(ast::idx idx_declaration_declarator)
{
  switch (tree[idx_declaration_declarator].value.declaration_declarator.type)
  {
    case ast::declarator_type::type_struct:
      return spt::Type::getStructTy();
    case ast::declarator_type::type_char:
      return spt::Type::getInt8Ty();
    case ast::declarator_type::type_double:
      return spt::Type::getDoubleTy();
    case ast::declarator_type::type_float:
      return spt::Type::getFloatTy();
    case ast::declarator_type::type_short_int:
      return spt::Type::getInt16Ty();
    case ast::declarator_type::type_int:
    case ast::declarator_type::type_long_int:
      return spt::Type::getInt32Ty();
    case ast::declarator_type::type_long_long_int:
      return spt::Type::getInt64Ty();
    // TODO void type
    case ast::declarator_type::type_void:
      return spt::Type::getVoid();
    default:
      SWITCH_ERROR
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @param idx_initial_declarator
 */
void spec_tree::trans_declaration_or_definition(
  ast::idx idx_declaration_declarator,
  ast::idx idx_initial_declarator
)
{
  ast::idx idx_declarator
    = tree[idx_initial_declarator].value.initial_declarator.idx_declarator;
  ast::idx idx_direct_declarator
    = tree[idx_declarator].value.declarator.idx_direct_declarator;

  spt::Type *ptr_type_declaration_declarator
    = trans_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  spt::Type *ptr_type = trans_pointer(ptr_type_declaration_declarator, idx_declarator);

  std::string identifier_name = get_identifier_name(idx_declarator);

  // ----------------------------------
  // array or a pointer to array
  // ----------------------------------
  if (tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null)
  {
    spt::Type *ptr_unit_type = ptr_type;
    ast::idx   idx_array_declarator
      = tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    spt::ArrayType *ptr_array_type = trans_array(ptr_unit_type, idx_array_declarator);

    // a pointer to array
    while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null
    )
    {
      idx_declarator = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
      trans_pointer(ptr_array_type, idx_declarator);
      idx_direct_declarator = tree[idx_declarator].value.declarator.idx_direct_declarator;
    }
    return;
  }
  // ----------------------------------
  // function
  // ----------------------------------
  else if (tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null)
  {
    spt::Type *ptr_return_type = ptr_type;
    // get function type list
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list
      = trans_arguments_type_list(
        tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
      );

    spt::FunctionType *ptr_func_type
      = spt::FunctionType::get(ptr_return_type, argument_type_list);

    /// 1. a pointer to function
    if (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null)
    {
      while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator
             != ast::null)
      {
        idx_declarator
          = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
        trans_pointer(ptr_func_type, idx_declarator);
        idx_direct_declarator
          = tree[idx_declarator].value.declarator.idx_direct_declarator;
      }
    }
    // 2. a function
    else
    {
      // 3. a function definition
      if (now_compound_statement != ast::null)
      {
        // compound_statement
        spt::FuncDef::create(
          ptr_return_type, argument_type_list, identifier_name,
          trans_compound_statement(now_compound_statement)
        );
      }
      // 4. a function declaration
      else
      {
        spt::FuncDec::create(ptr_return_type, identifier_name, argument_type_list);
      }
    }
  }
  // ----------------------------------
  // struct enum union
  // ----------------------------------
  else if (tree[idx_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_struct)
  {
    // struct definition
    if (now_compound_statement != ast::null)
    {
      spt::StructDef::create(
        identifier_name, trans_struct_element_type_list(now_compound_statement)
      );
    }
    // struct declaration
    else
    {
      spt::StructDec::create(identifier_name);
    }
  }
  // ----------------------------------
  // var
  // ----------------------------------
  else
  {
    // exist initializer
    if (tree[idx_initial_declarator].value.initial_declarator.idx_initializer != ast::null)
    {
      ast::idx idx_initializer
        = tree[idx_initial_declarator].value.initial_declarator.idx_initializer;
      ast::idx idx_assignment_expression
        = tree[idx_initializer].value.initializer.idx_assignment_expression;
      spt::VarDef::create(
        ptr_type, identifier_name, trans_expression(idx_assignment_expression)
      );
    }
    // not exist initializer
    else
    {
      spt::VarDef::create(ptr_type, identifier_name);
    }
  }
  now_compound_statement = ast::null;
}

/**
 * @brief
 *
 * @param ptr_type_declaration_declarator
 * @param idx_declarator
 * @return spt::Type*
 */
spt::Type *spec_tree::trans_pointer(
  spt::Type *ptr_type_declaration_declarator,
  ast::idx   idx_declarator
)
{
  TODO for (auto i = tree[idx_declarator].value.declarator.is_ptr; i > 0; --i)
  {
    // ptr_type_declaration_declarator
    //   = spt::PointerType::get(ptr_type_declaration_declarator, 0);
  }
  return ptr_type_declaration_declarator;
}


/**
 * @brief Get the identifier name
 *
 * @param idx_declarator
 * @return std::string
 */
std::string spec_tree::get_identifier_name(ast::idx idx_declarator)
{
  ast::idx idx_identifier = ast::null;
  for (ast::idx i = idx_declarator; tree[i].type != ast::node_type::identifier;)
  {
    switch (tree[i].type)
    {
      case ast::node_type::declarator:
        i = tree[i].value.declarator.idx_direct_declarator;
        break;
      case ast::node_type::direct_declarator:
        if (tree[i].value.direct_declarator.idx_identifier != ast::null)
        {
          i              = tree[i].value.direct_declarator.idx_identifier;
          idx_identifier = i;
        }
        else
        {
          i = tree[i].value.direct_declarator.idx_declarator;
        }
        break;
      default:
        fmt::print(
          fg(fmt::color::red), "node type {}\n", static_cast<unsigned char>(tree[i].type)
        );
        SWITCH_ERROR
    }
  }
  return std::string(tree[idx_identifier].value.identifier.name);
}

/**
 * @brief
 *
 * @param ptr_unit_type
 * @param idx_array_declarator
 * @return spt::ArrayType*
 */
spt::ArrayType *
spec_tree::trans_array(spt::Type *ptr_unit_type, ast::idx idx_array_declarator)
{
  ast::idx idx_next_array_declarator
    = tree[idx_array_declarator].value.array_declarator.idx_next_array_declarator;
  std::vector<std::size_t> dimension_len;
  for (ast::idx i = idx_next_array_declarator; i != ast::null;
       i          = tree[i].value.array_declarator.idx_next_array_declarator)
  {
    ast::idx idx_constant
      = tree[idx_array_declarator].value.array_declarator.idx_constant;
    dimension_len.push_back(constant_node_to_uint64(idx_constant));
  }
  return spt::ArrayType::get(ptr_unit_type, dimension_len);
}

}  // namespace toy_c