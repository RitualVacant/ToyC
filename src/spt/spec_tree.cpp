#ifndef SPEC_TREE_CPP
#define SPEC_TREE_CPP

#include "spec_tree.h"
#include "ast_node.h"
#include "fstream_guard.h"
#include "inner.h"
#include "parser.h"
#include "spec_tree_node.h"
#include <fmt/color.h>
#include <iostream>
#include <string>

namespace spt
{

/**
 * @brief Construct a new spec ast::spec ast object
 *
 * @param ast
 */
Tree::Tree()
{
  toy_c::parser parser;
  ast = std::move(parser.get_ast_tree());
  symbol_table.push_scope();
  ptr_root_tree_body = build_multi_declaration_or_definition(1);
  symbol_table.pop_scope();
}

/**
 * @brief Destroy the spec ast::spec ast object
 *
 */


/**
 * @brief
 *
 */
void Tree::print_spec_tree()
{
  json.init();
  ptr_root_tree_body->print();
  json.end();
  toy_c::fstream_guard file(output_file_path, toy_c::mode::write);
  file << json.get_str_ref();
}


/**
 * @brief
 *
 */
void Tree::generate_llvm_ir()
{
  ptr_root_tree_body->gen();
}


/**
 * @brief
 *
 * @param idx_declaration_or_definition
 * @return Block*
 */
Block *Tree::build_multi_declaration_or_definition(ast::idx idx_declaration_or_definition)
{
  //
  // idx_declaration_declarator
  //          ||
  //          ||        j
  //          ||       ||
  //          \/       \/
  //  i->    int a, b, c = 3, *d;
  //

  Block *block = Block::create();

  for (ast::idx i = idx_declaration_or_definition,
                idx_declaration_declarator
                = ast[i].value.declaration_or_definition.idx_declaration_declarator;
       i != ast::null;
       i = ast[i].value.declaration_or_definition.idx_next_declaration_or_definition,
                idx_declaration_declarator
                = ast[i].value.declaration_or_definition.idx_declaration_declarator)
  {
    now_compound_statement
      = ast[i].value.declaration_or_definition.idx_compound_statement;

    for (ast::idx j = ast[i].value.declaration_or_definition.idx_initial_declarator;
         j != ast::null; j = ast[j].value.initial_declarator.idx_next_initial_declarator)
    {
      block->push_back(build_declaration_or_definition(idx_declaration_declarator, j));
      // now_compound_statement = ast::null;
    }
  }

  return block;
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @return Type*
 */
Type *Tree::build_declaration_declarator(ast::idx idx_declaration_declarator)
{
  // unsigned
  if (ast[idx_declaration_declarator].value.declaration_declarator.sign == ast::declarator_sign::sign_signed)
  {
    switch (ast[idx_declaration_declarator].value.declaration_declarator.type)
    {
      case ast::declarator_type::type_struct:
        return Type::get_struct();
      case ast::declarator_type::type_char:
        return Type::get_int8();
      case ast::declarator_type::type_double:
        return Type::get_double();
      case ast::declarator_type::type_float:
        return Type::get_float();
      case ast::declarator_type::type_short_int:
        return Type::get_int16();
      case ast::declarator_type::type_int:
      case ast::declarator_type::type_long_int:
        return Type::get_int32();
      case ast::declarator_type::type_long_long_int:
        return Type::get_int64();
      case ast::declarator_type::type_void:
        return Type::get_void();
      default:
        fmt::print(
          fg(fmt::color::red), "declarator type: {}\n",
          static_cast<unsigned char>(
            ast[idx_declaration_declarator].value.declaration_declarator.type
          )
        );
        SWITCH_ERROR
    }
  }
  else
  {
    switch (ast[idx_declaration_declarator].value.declaration_declarator.type)
    {
      case ast::declarator_type::type_double:
        // TODO unsigned double
        return Type::get_udouble();
      case ast::declarator_type::type_float:
        return Type::get_ufloat();
      case ast::declarator_type::type_char:
        return Type::get_uint8();
      case ast::declarator_type::type_short_int:
        return Type::get_uint16();
      case ast::declarator_type::type_int:
      case ast::declarator_type::type_long_int:
        return Type::get_uint32();
      case ast::declarator_type::type_long_long_int:
        return Type::get_uint64();
      default:
        fmt::print(
          fg(fmt::color::red), "declarator type: {}\n",
          static_cast<unsigned char>(
            ast[idx_declaration_declarator].value.declaration_declarator.type
          )
        );
        SWITCH_ERROR
    }
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @param idx_initial_declarator
 */
Statement *Tree::build_declaration_or_definition(
  ast::idx idx_declaration_declarator,
  ast::idx idx_initial_declarator
)
{
  ast::idx idx_declarator
    = ast[idx_initial_declarator].value.initial_declarator.idx_declarator;
  ast::idx idx_direct_declarator
    = ast[idx_declarator].value.declarator.idx_direct_declarator;

  Type *ptr_type_declaration_declarator
    = build_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

  std::string identifier_name = get_identifier_name(idx_declarator);

  // ----------------------------------
  // array or a pointer to array
  // ----------------------------------
  if (ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null)
  {
    Type    *ptr_unit_type = ptr_type;
    ast::idx idx_array_declarator
      = ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    ArrayType *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

    // a pointer to array
    while (ast[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null)
    {
      idx_declarator = ast[idx_direct_declarator].value.direct_declarator.idx_declarator;
      build_pointer(ptr_array_type, idx_declarator);
      idx_direct_declarator = ast[idx_declarator].value.declarator.idx_direct_declarator;
    }

    // exist initializer
    if (ast[idx_initial_declarator].value.initial_declarator.idx_initializer != ast::null)
    {
      ast::idx idx_initializer
        = ast[idx_initial_declarator].value.initial_declarator.idx_initializer;

      return ArrayDef::create(
        ptr_array_type, identifier_name, build_initializer(idx_initializer)
      );
    }
    // not exist initializer
    else
    {
      return ArrayDef::create(ptr_array_type, identifier_name, nullptr);
    }
  }
  // ----------------------------------
  // function
  // ----------------------------------
  else if (ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null)
  {
    Type *ptr_return_type = ptr_type;
    // get function type list
    std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list
      = build_arguments_type_list(
        ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
      );

    FuncType *ptr_func_type
      = FuncType::get(ptr_return_type, std::get<0>(argument_type_list));

    /// 1. a pointer to function
    if (ast[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null)
    {
      while (ast[idx_direct_declarator].value.direct_declarator.idx_declarator
             != ast::null)
      {
        idx_declarator
          = ast[idx_direct_declarator].value.direct_declarator.idx_declarator;
        build_pointer(ptr_func_type, idx_declarator);
        idx_direct_declarator
          = ast[idx_declarator].value.declarator.idx_direct_declarator;
      }
    }
    // 2. a function
    else
    {
      // 3. a function definition
      if (now_compound_statement != ast::null)
      {
        // compound_statement
        return FuncDef::create(
          ptr_return_type, identifier_name, argument_type_list,
          build_compound_statement(now_compound_statement)
        );
      }
      // 4. a function declaration
      else
      {
        return FuncDec::create(ptr_return_type, identifier_name, argument_type_list);
      }
    }
  }
  // ----------------------------------
  // struct enum union
  // ----------------------------------
  else if (ast[idx_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_struct)
  {
    {
      TODO
    }
    // struct definition
    if (now_compound_statement != ast::null)
    {
      return StructDef::create(
        identifier_name, build_struct_element_type_list(now_compound_statement)
      );
    }
    // struct declaration
    else
    {
      return StructDec::create(identifier_name);
    }
  }
  // ----------------------------------
  // basic variable
  // ----------------------------------
  else
  {
    ast::idx idx_initializer
      = ast[idx_initial_declarator].value.initial_declarator.idx_initializer;
    ast::idx idx_assignment_expression
      = ast[idx_initializer].value.initializer.idx_assignment_expression;
    symbol_table.insert_symbol(identifier_name, ptr_type);
    return VarDef::create(
      ptr_type, identifier_name, build_expression(idx_assignment_expression)
    );
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_initializer
 * @return Initializer*
 */
Initializer *Tree::build_initializer(ast::idx idx_initializer)
{
  return Initializer::create(build_initializer_node_list(idx_initializer));
}

InitializerNode *Tree::build_initializer_node(ast::idx idx_initializer_node)
{
  if (idx_initializer_node == ast::null)
  {
    return nullptr;
  }
  return InitializerNode::create(
    build_constant(ast[idx_initializer_node].value.initializer_list_node.idx_constant),
    build_initializer_node(
      ast[idx_initializer_node].value.initializer_list_node.idx_next_initializer_list_node
    )
  );
}
InitializerNodeList *Tree::build_initializer_node_list(ast::idx idx_initializer_node_list)
{
  if (idx_initializer_node_list == ast::null)
  {
    return nullptr;
  }
  return InitializerNodeList::create(
    build_initializer_node(
      ast[idx_initializer_node_list].value.initializer_list.idx_head_initializer_list_node
    ),
    build_initializer_node_list(
      ast[idx_initializer_node_list].value.initializer_list.idx_son_initializer_list
    ),
    build_initializer_node_list(
      ast[idx_initializer_node_list].value.initializer_list.idx_next_initializer_list
    )
  );
}

/**
 * @brief
 *
 * @param ptr_type_declaration_declarator
 * @param idx_declarator
 * @return Type*
 */
Type *Tree::build_pointer(Type *ptr_type_declaration_declarator, ast::idx idx_declarator)
{
  for (auto i = ast[idx_declarator].value.declarator.is_ptr; i > 0; --i)
  {
    ptr_type_declaration_declarator = PointerType::get(ptr_type_declaration_declarator);
  }
  return ptr_type_declaration_declarator;
}


/**
 * @brief Get the identifier name
 *
 * @param idx_declarator
 * @return std::string
 */
std::string Tree::get_identifier_name(ast::idx idx_declarator)
{
  ast::idx idx_identifier = ast::null;
  for (ast::idx i = idx_declarator; ast[i].type != ast::node_type::identifier;)
  {
    switch (ast[i].type)
    {
      case ast::node_type::declarator:
        i = ast[i].value.declarator.idx_direct_declarator;
        break;
      case ast::node_type::direct_declarator:
        if (ast[i].value.direct_declarator.idx_identifier != ast::null)
        {
          i              = ast[i].value.direct_declarator.idx_identifier;
          idx_identifier = i;
        }
        else
        {
          i = ast[i].value.direct_declarator.idx_declarator;
        }
        break;
      default:
        fmt::print(
          fg(fmt::color::red), "node type {}\n", static_cast<unsigned char>(ast[i].type)
        );
        SWITCH_ERROR
    }
  }
  return std::string(ast[idx_identifier].value.identifier.name);
}

/**
 * @brief
 *
 * @param ptr_unit_type
 * @param idx_array_declarator
 * @return ArrayType*
 */
ArrayType *Tree::build_array(Type *ptr_unit_type, ast::idx idx_array_declarator)
{
  std::vector<Expr *> dimension_len_expr;
  for (ast::idx i = idx_array_declarator; i != ast::null;
       i          = ast[i].value.array_declarator.idx_next_array_declarator)
  {
    dimension_len_expr.push_back(
      build_expression(ast[i].value.array_declarator.idx_constant)
    );
  }
  return ArrayType::get(ptr_unit_type, dimension_len_expr);
}

/**
 * @brief
 *
 * @param idx_constant
 * @return std::uint64_t
 */
std::uint64_t Tree::constant_node_to_uint64(ast::idx idx_constant)
{
  return std::stoull(ast.get_constant(idx_constant).value);
}

/**
 * @brief
 *
 * @param idx_compound_statement
 * @return Block*
 */
Block *Tree::build_compound_statement(ast::idx idx_compound_statement)
{
  Block   *block     = Block::create();
  ast::idx idx_block = ast[idx_compound_statement].value.compound_statement.idx_block;

  symbol_table.push_scope();

  for (ast::idx i = idx_block; i != ast::null; i = ast[i].value.block.idx_next_block)
  {
    ast::idx idx_declaration = ast[i].value.block.idx_declaration;
    ast::idx idx_statement   = ast[i].value.block.idx_statement;
    if (idx_declaration != ast::null)
    {
      block->push_back(build_multi_declaration_or_definition(idx_declaration));
    }
    else
    {
      block->push_back(build_statement(idx_statement));
    }
  }

  symbol_table.pop_scope();
  return block;
}

/**
 * @brief
 *
 * @param idx_idx_block
 * @return Block*
 */
Statement *Tree::build_statement(ast::idx idx_statement)
{
  switch (ast[idx_statement].type)
  {
    case ast::node_type::if_statement:
      return build_if_statement(idx_statement);
    case ast::node_type::switch_statement:
      return build_switch_statement(idx_statement);
    case ast::node_type::return_statement:
      return build_return_statement(idx_statement);
    case ast::node_type::while_statement:
      return build_while_statement(idx_statement);
    case ast::node_type::do_while_statement:
      return build_do_while_statement(idx_statement);
    case ast::node_type::for_statement:
      return build_for_statement(idx_statement);
    case ast::node_type::expression:
      return build_expression(idx_statement);
    default:
      SWITCH_ERROR
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_expression
 * @return Expr*
 */
// idx expression 的种类
Expr *Tree::build_expression(ast::idx idx_expression)
{
  if (idx_expression == ast::null)
  {
    return nullptr;
  }

  switch (ast[idx_expression].type)
  {
    case ast::node_type::expression:
      return build_expression(
        ast[idx_expression].value.expression.idx_assignment_expression
      );
    case ast::node_type::assignment_expression:
      return build_assign_expression(idx_expression);
    case ast::node_type::binary_expression:
      return build_binary_expression(idx_expression);
    case ast::node_type::conditional_expression:
      return build_conditional_expression(idx_expression);
    case ast::node_type::unary_expression:
      return build_unary_expression(idx_expression);
    default:
      SWITCH_ERROR
  }
  NOT_REACHABLE
}

Expr *Tree::build_binary_expression(ast::idx idx_binary_expression)
{
  return Expr::create(
    ast[idx_binary_expression].value.binary_expression.token_operator,
    build_expression(ast[idx_binary_expression].value.binary_expression.idx_left_node),
    build_expression(ast[idx_binary_expression].value.binary_expression.idx_right_node)
  );
}

Expr *Tree::build_assign_expression(ast::idx idx_assign_expression)
{
  ast::idx idx_binary_expression
    = ast[idx_assign_expression].value.assignment_expression.idx_binary_expression;

  // just a binary expression without assign
  Expr *r_value = build_expression(idx_binary_expression);
  Expr *l_value;

  for (ast::idx i = idx_assign_expression; i != ast::null;
       i          = ast[i].value.assignment_expression.idx_next_assignment_expression)
  {
    l_value = build_unary_expression(
      ast[i].value.assignment_expression.idx_unary_or_binary_expression
    );
    r_value = Expr::create(
      ast[i].value.assignment_expression.assignment_type, l_value, r_value
    );
  }
  return r_value;
}


Expr *Tree::build_unary_expression(ast::idx idx_unary_expression)
{
  //----------------------------------------------------------------
  // 1. sizeof return a constant
  //----------------------------------------------------------------
  if (ast[idx_unary_expression].value.unary_expression.is_sizeof)
  {
    {
      TODO
    }
    // unary_expression
    if (ast[idx_unary_expression].type == ast::node_type::unary_expression)
    {
      return build_compute_unary_expression_size(
        ast[idx_unary_expression].value.unary_expression.idx_unary_expression
      );
    }
    // declarator
    else
    {
      return build_compute_declarator_size(
        ast[idx_unary_expression].value.unary_expression.idx_declaration_declarator
      );
    }
  }

  ast::idx idx_postfix_expression
    = ast[idx_unary_expression].value.unary_expression.idx_postfix_expression;

  ast::idx idx_primary_expression
    = ast[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //----------------------------------------------------------------
  // 2. unary operator
  //----------------------------------------------------------------
  if (ast[idx_unary_expression].value.unary_expression.unary_operator != token::invalid)
  {
    return spt::UnaryExpr::create(
      ast[idx_unary_expression].value.unary_expression.unary_operator,
      build_unary_expression(
        ast[idx_unary_expression].value.unary_expression.idx_unary_expression
      )
    );
  }
  //----------------------------------------------------------------
  // 3. postfix expression
  //----------------------------------------------------------------
  if (idx_postfix_expression != ast::null)
  {
    return build_postfix_expression(idx_postfix_expression);
  }
  //----------------------------------------------------------------
  // 4. declaration declarator : conversion
  //----------------------------------------------------------------
  if (ast[idx_unary_expression].value.unary_expression.idx_declaration_declarator != ast::null)
  {
    return build_conversion(idx_unary_expression);
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_unary_expression
 * @return Expr*
 */
Expr *Tree::build_conversion(ast::idx idx_unary_expression){TODO}

/**
 * @brief
 *
 * @param idx_conditional_expression
 * @return Expr*
 */
Expr *Tree::build_conditional_expression(ast::idx idx_conditional_expression){TODO}


/**
 * @brief
 *
 * @param idx_postfix_expression
 * @return Expr*
 */
Expr *Tree::build_postfix_expression(ast::idx idx_postfix_expression)
{
  ast::idx idx_primary_expression
    = ast[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //------------
  // identifier : array func call and other with identifier
  //------------
  if (ast[idx_primary_expression].value.primary_expression.idx_identifier != ast::null)
  {
    ast::idx idx_identifier
      = ast[idx_primary_expression].value.primary_expression.idx_identifier;
    std::string identifier = std::string(ast[idx_identifier].value.identifier.name);

    // exist postfix operator
    if (ast[idx_postfix_expression].value.postfix_expression.idx_postfix_operator != ast::null)
    {
      ast::idx idx_postfix_operator
        = ast[idx_postfix_expression].value.postfix_expression.idx_postfix_operator;

      // array [] or function call ()
      if (ast[idx_postfix_operator].value.postfix_operator.postfix_operator == token::invalid)
      {
        // array
        if (ast[idx_postfix_operator].value.postfix_operator.idx_array_idx_assignment_expression != ast::null)
        {
          std::vector<Expr *> idx_list;
          for (; idx_postfix_operator != ast::null;
               idx_postfix_operator = ast[idx_postfix_operator]
                                        .value.postfix_operator.idx_next_postfix_operator)
          {
            idx_list.push_back(build_expression(
              ast[idx_postfix_operator]
                .value.postfix_operator.idx_array_idx_assignment_expression
            ));
          }
          return Array::create(identifier, idx_list);
        }
        // function call
        else if (ast[idx_postfix_operator].value.postfix_operator.idx_func_call_assignment_expression_list != ast::null)
        {
          return FuncCall::create(
            identifier, build_argument_list(ast[idx_postfix_operator]
                                              .value.postfix_operator
                                              .idx_func_call_assignment_expression_list)
          );
        }
      }
      // ++ -- -> .
      else
      {
        return Var::create(
          identifier, symbol_table.get_type(identifier),
          build_postfix_operator(idx_postfix_operator)
        );
      }
    }
    // not exist postfix operator
    else
    {
      return Var::create(identifier, symbol_table.get_type(identifier));
    }
  }
  //------------
  // constant
  //------------
  else if (ast[idx_primary_expression].value.primary_expression.idx_constant != ast::null)
  {
    return build_constant(
      ast[idx_primary_expression].value.primary_expression.idx_constant
    );
  }
  //------------
  // expression
  //------------
  else
  {
    return build_binary_expression(idx_postfix_expression);
  }
  NOT_REACHABLE
}


/**
 * @brief
 *
 * @param idx_postfix_operator
 * @return PostfixOperator*
 */
PostfixOperator *Tree::build_postfix_operator(ast::idx idx_postfix_operator)
{
  if (idx_postfix_operator == ast::null)
  {
    return nullptr;
  }
  ast::idx idx_identifier
    = ast[idx_postfix_operator].value.postfix_operator.idx_identifier;
  ast::idx idx_next_postfix_operator
    = ast[idx_postfix_operator].value.postfix_operator.idx_next_postfix_operator;
  switch (ast[idx_postfix_operator].value.postfix_operator.postfix_operator)
  {
    // ++
    case token::self_plus:
      return SelfPulsOperator::create(build_postfix_operator(idx_next_postfix_operator));
    // --
    case token::self_minus:
      return SelfMinusOperator::create(build_postfix_operator(idx_next_postfix_operator));
    // ->
    case token::ver:
      return StructOperatorPtr::create(
        ast[idx_postfix_operator].value.identifier.name,
        build_postfix_operator(idx_next_postfix_operator)
      );
    // .
    case token::period:
      return StructOperator::create(
        ast[idx_postfix_operator].value.identifier.name,
        build_postfix_operator(idx_next_postfix_operator)
      );
    // array [] or function call ()
    case token::invalid: {
      // array
      if (ast[idx_postfix_operator].value.postfix_operator.idx_array_idx_assignment_expression != ast::null)
      {
        std::vector<Expr *> idx_list;
        for (; idx_postfix_operator != ast::null
               && ast[idx_postfix_operator]
                      .value.postfix_operator.idx_array_idx_assignment_expression
                    != ast::null;

             idx_postfix_operator
             = ast[idx_postfix_operator].value.postfix_operator.idx_next_postfix_operator)
        {
          idx_list.push_back(build_expression(
            ast[idx_postfix_operator]
              .value.postfix_operator.idx_array_idx_assignment_expression
          ));
        }
        return ArrayOperator::create(
          idx_list, build_postfix_operator(idx_postfix_operator)
        );
      }
      // function call
      else if (ast[idx_postfix_operator].value.postfix_operator.idx_func_call_assignment_expression_list != ast::null)
      {
        return FuncOperator::create(
          build_argument_list(
            ast[idx_postfix_operator]
              .value.postfix_operator.idx_func_call_assignment_expression_list
          ),
          build_postfix_operator(idx_next_postfix_operator)
        );
      }
    }
    default:
      SWITCH_ERROR
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_expression_list
 * @return std::vector<Expr *>
 */
std::vector<Expr *> Tree::build_argument_list(ast::idx idx_expression_list){TODO}

/**
 * @brief
 *
 * @param idx_arguments_type_list
 * @return std::tuple<std::vector<Type *>, std::vector<std::string>>
 */
std::tuple<std::vector<Type *>, std::vector<std::string>> Tree::build_arguments_type_list(
  ast::idx idx_arguments_type_list
)
{
  std::tuple<std::vector<Type *>, std::vector<std::string>> arguments_type_list;
  for (ast::idx i
       = ast[idx_arguments_type_list].value.arguments_type_list.idx_argument_declaration;
       i != ast::null;
       i = ast[i].value.arguments_declaration.idx_next_arguments_declaration)
  {
    std::get<0>(arguments_type_list)
      .push_back(std::get<0>(build_argument_declaration(i)));
    std::get<1>(arguments_type_list)
      .push_back(std::get<1>(build_argument_declaration(i)));
  }

  return arguments_type_list;
}

/**
 * @brief
 *
 * @param idx_argument_declaration
 * @return std::tuple<Type *, std::string>
 */
std::tuple<Type *, std::string>
Tree::build_argument_declaration(ast::idx idx_argument_declaration)
{
  ast::idx idx_declaration_declarator
    = ast[idx_argument_declaration]
        .value.arguments_declaration.idx_declaration_declarator;

  ast::idx idx_declarator
    = ast[idx_argument_declaration].value.arguments_declaration.idx_declarator;

  Type *ptr_type_argument = build_type(idx_declaration_declarator, idx_declarator);

  return {ptr_type_argument, get_identifier_name(idx_declarator)};
}


/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @param idx_declarator
 * @return Type*
 */

Type *Tree::build_type(ast::idx idx_declaration_declarator, ast::idx idx_declarator)
{
  Type *ptr_type_declaration_declarator
    = build_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

  ast::idx idx_direct_declarator
    = ast[idx_declarator].value.declarator.idx_direct_declarator;

  // array or a pointer to array
  if (ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null)
  {
    Type    *ptr_unit_type = ptr_type;
    ast::idx idx_array_declarator
      = ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    Type *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

    // a pointer to array
    ast::idx idx_declarator;
    while (ast[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null)
    {
      idx_declarator = ast[idx_direct_declarator].value.direct_declarator.idx_declarator;
      build_pointer(ptr_array_type, idx_declarator);
      idx_direct_declarator = ast[idx_declarator].value.declarator.idx_direct_declarator;
    }

    return ptr_array_type;
  }
  // TODO
  // function
  else if (ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null)
  {
    Type *ptr_return_type = ptr_type;
    // get function type list
    std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list
      = build_arguments_type_list(
        ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
      );

    FuncType *ptr_func_type
      = FuncType::get(ptr_return_type, std::get<0>(argument_type_list));
    return ptr_func_type;
  }
  // var
  else
  {
    return ptr_type;
  }

  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_compound_statement
 * @return std::tuple<std::vector<Type *>, std::vector<std::string>>
 */
std::tuple<std::vector<Type *>, std::vector<std::string>>
Tree::build_struct_element_type_list(ast::idx idx_compound_statement){TODO}

/**
 * @brief
 *
 * @param idx_if_statement
 */
Statement *Tree::build_if_statement(ast::idx idx_if_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_if_statement].value.if_statement.idx_assign_expression;
  ast::idx idx_else_body = ast[idx_if_statement].value.if_statement.idx_else_body;
  ast::idx idx_if_body   = ast[idx_if_statement].value.if_statement.idx_if_body;

  // loop conditional
  Expr *expr = build_expression(idx_assign_expression);
  // if body
  Block *true_block = build_compound_statement(idx_if_body);
  // else body
  if (idx_else_body != ast::null)
  {
    return IfStatement::create(expr, true_block);
  }
  else
  {
    Block *false_block = build_compound_statement(idx_else_body);
    return IfStatement::create(expr, true_block, false_block);
  }
}

/**
 * @brief
 *
 * @param idx_statement
 * @return Statement*
 */
Statement *Tree::build_switch_statement(ast::idx idx_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_statement].value.switch_statement.idx_assign_expression;
  ast::idx idx_compound_statement
    = ast[idx_statement].value.switch_statement.idx_compound_statement;
  TODO
}

/**
 * @brief
 *
 * @param idx_statement
 * @return Statement*
 */
Statement *Tree::build_return_statement(ast::idx idx_return_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_return_statement].value.return_statement.idx_assignment_expression;

  return ReturnStatement::create(build_expression(idx_assign_expression));
}

/**
 * @brief
 *
 * @param idx_statement
 * @return Statement*
 */
Statement *Tree::build_while_statement(ast::idx idx_while_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_while_statement].value.while_statement.idx_assignment_expression;
  ast::idx idx_compound_statement
    = ast[idx_while_statement].value.while_statement.idx_compound_statement;

  return WhileStatement::create(
    build_expression(idx_assign_expression),
    build_compound_statement(idx_compound_statement)
  );
}

/**
 * @brief
 *
 * @param idx_statement
 * @return Statement*
 */
Statement *Tree::build_do_while_statement(ast::idx idx_do_while_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_do_while_statement].value.do_while_statement.idx_assign_statement;
  ast::idx idx_compound_statement
    = ast[idx_do_while_statement].value.do_while_statement.idx_compound_statement;

  return DoWhileStatement::create(
    build_expression(idx_assign_expression),
    build_compound_statement(idx_compound_statement)
  );
}

/**
 * @brief
 *
 * @param idx_statement
 * @return Statement*
 */
Statement *Tree::build_for_statement(ast::idx idx_for_statement)
{
  ast::idx idx_change_assign_expression
    = ast[idx_for_statement].value.for_statement.idx_change_assign_expression;

  ast::idx idx_compound_statement
    = ast[idx_for_statement].value.for_statement.idx_compound_statement;

  ast::idx idx_conditional_assign_expression
    = ast[idx_for_statement].value.for_statement.idx_conditional_assign_expression;

  ast::idx idx_declaration = ast[idx_for_statement].value.for_statement.idx_declaration;

  /*
    the order of function execute is undefine in standard
    ----------------------------------------------------
    return ForStatement::create(
      // declarations
      build_multi_declaration_or_definition(idx_declaration),
      // condition
      build_expression(idx_conditional_assign_expression),
      // change value
      build_expression(idx_change_assign_expression),
      // compound statement
      build_compound_statement(idx_compound_statement)
    );
  */

  symbol_table.push_scope();
  Block *init_block     = build_multi_declaration_or_definition(idx_declaration);
  Expr  *condition_expr = build_expression(idx_conditional_assign_expression);
  Expr  *change_expr    = build_expression(idx_change_assign_expression);
  Block *for_body       = build_compound_statement(idx_compound_statement);
  symbol_table.pop_scope();
  return ForStatement::create(init_block, condition_expr, change_expr, for_body);
}

Expr *Tree::build_compute_unary_expression_size(ast::idx unary_expression){TODO}

Expr *Tree::build_compute_declarator_size(ast::idx idx_declaration_declarator){TODO}

/**
 * @brief
 *
 * @param idx_constant
 * @return Constant*
 */
Constant *Tree::build_constant(ast::idx idx_constant)
{
  switch (ast.get_constant(idx_constant).type)
  {
    case ast::node_type::constant_string:
      return StringLiteral::create(ast.get_constant(idx_constant).value);
    case ast::node_type::constant_negative_float_number:
      return ConstantFloat::create(ast.get_constant(idx_constant).value, true);
    case ast::node_type::constant_float_number:
      return ConstantFloat::create(ast.get_constant(idx_constant).value, false);
    case ast::node_type::constant_integer_number:
      return ConstantInt::create(ast.get_constant(idx_constant).value, false);
    case ast::node_type::constant_negative_integer_number:
      return ConstantInt::create(ast.get_constant(idx_constant).value, true);
    default:
      SWITCH_ERROR
  }
}

}  // namespace spt

#endif
