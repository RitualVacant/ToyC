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

// std::vector<spt::spec_tree_node> *ptr_tree_body = nullptr;

/**
 * @brief Construct a new spec ast::spec ast object
 *
 * @param ast
 */
spt::Tree::Tree()
{
  toy_c::parser parser;
  this->ast          = std::move(parser.get_ast_tree());
  ptr_root_tree_body = build_multi_declaration_or_definition(1);
}

/**
 * @brief Destroy the spec ast::spec ast object
 *
 */
// spt::Tree::~Tree() {}


/**
 * @brief
 *
 */
void spt::Tree::print_spec_tree()
{
  ptr_root_tree_body->print();
  toy_c::fstream_guard file(output_file_path, toy_c::mode::write);
  file << json.get_str_ref();
}

/**
 * @brief
 *
 */
spt::Block *
spt::Tree::build_multi_declaration_or_definition(ast::idx idx_declaration_or_definition)
{
  //
  // idx_declaration_declarator
  //          ||
  //          ||        j
  //          ||       ||
  //          \/       \/
  //  i->    int a, b, c = 3, *d;
  //

  spt::Block *block = Block::create();

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
      now_compound_statement = ast::null;
    }
  }

  return block;
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @return spt::Type*
 */
spt::Type *spt::Tree::build_declaration_declarator(ast::idx idx_declaration_declarator)
{
  // unsign
  if (ast[idx_declaration_declarator].value.declaration_declarator.sign == ast::declarator_sign::sign_signed)
  {
    switch (ast[idx_declaration_declarator].value.declaration_declarator.type)
    {
      case ast::declarator_type::type_struct:
        return spt::Type::get_struct();
      case ast::declarator_type::type_char:
        return spt::Type::get_int8();
      case ast::declarator_type::type_double:
        return spt::Type::get_double();
      case ast::declarator_type::type_float:
        return spt::Type::get_float();
      case ast::declarator_type::type_short_int:
        return spt::Type::get_int16();
      case ast::declarator_type::type_int:
      case ast::declarator_type::type_long_int:
        return spt::Type::get_int32();
      case ast::declarator_type::type_long_long_int:
        return spt::Type::get_int64();
      case ast::declarator_type::type_void:
        return spt::Type::get_void();
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
        TODO return spt::Type::get_double();
      case ast::declarator_type::type_float:
        TODO return spt::Type::get_float();
      case ast::declarator_type::type_char:
        return spt::Type::get_uint8();
      case ast::declarator_type::type_short_int:
        return spt::Type::get_uint16();
      case ast::declarator_type::type_int:
      case ast::declarator_type::type_long_int:
        return spt::Type::get_uint32();
      case ast::declarator_type::type_long_long_int:
        return spt::Type::get_uint64();
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
spt::Statement *spt::Tree::build_declaration_or_definition(
  ast::idx idx_declaration_declarator,
  ast::idx idx_initial_declarator
)
{
  ast::idx idx_declarator
    = ast[idx_initial_declarator].value.initial_declarator.idx_declarator;
  ast::idx idx_direct_declarator
    = ast[idx_declarator].value.declarator.idx_direct_declarator;

  spt::Type *ptr_type_declaration_declarator
    = build_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  spt::Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

  std::string identifier_name = get_identifier_name(idx_declarator);

  // ----------------------------------
  // array or a pointer to array
  // ----------------------------------
  if (ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null)
  {
    spt::Type *ptr_unit_type = ptr_type;
    ast::idx   idx_array_declarator
      = ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    spt::ArrayType *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

    // a pointer to array
    while (ast[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null)
    {
      idx_declarator = ast[idx_direct_declarator].value.direct_declarator.idx_declarator;
      build_pointer(ptr_array_type, idx_declarator);
      idx_direct_declarator = ast[idx_declarator].value.declarator.idx_direct_declarator;
    }

    // TODO
    // exist initializer
    if (ast[idx_initial_declarator].value.initial_declarator.idx_initializer != ast::null)
    {
      ast::idx idx_initializer
        = ast[idx_initial_declarator].value.initial_declarator.idx_initializer;
      ast::idx idx_assignment_expression
        = ast[idx_initializer].value.initializer.idx_assignment_expression;

      return spt::VarDef::create(
        ptr_type, identifier_name, build_expression(idx_assignment_expression)
      );
    }
    // not exist initializer
    else
    {
      return spt::VarDef::create(ptr_type, identifier_name);
    }
  }
  // ----------------------------------
  // function
  // ----------------------------------
  else if (ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null)
  {
    spt::Type *ptr_return_type = ptr_type;
    // get function type list
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list
      = build_arguments_type_list(
        ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
      );

    spt::FuncType *ptr_func_type
      = spt::FuncType::get(ptr_return_type, std::get<0>(argument_type_list));

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
        return spt::FuncDef::create(
          ptr_return_type, identifier_name, argument_type_list,
          build_block(now_compound_statement)
        );
      }
      // 4. a function declaration
      else
      {
        return spt::FuncDec::create(ptr_return_type, identifier_name, argument_type_list);
      }
    }
  }
  // ----------------------------------
  // struct enum union
  // ----------------------------------
  else if (ast[idx_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_struct)
  {
    // struct definition
    if (now_compound_statement != ast::null)
    {
      return spt::StructDef::create(
        identifier_name, build_struct_element_type_list(now_compound_statement)
      );
    }
    // struct declaration
    else
    {
      return spt::StructDec::create(identifier_name);
    }
  }
  // ----------------------------------
  // basic var
  // ----------------------------------
  else
  {
    // exist initializer
    if (ast[idx_initial_declarator].value.initial_declarator.idx_initializer != ast::null)
    {
      ast::idx idx_initializer
        = ast[idx_initial_declarator].value.initial_declarator.idx_initializer;
      ast::idx idx_assignment_expression
        = ast[idx_initializer].value.initializer.idx_assignment_expression;
      return spt::VarDef::create(
        ptr_type, identifier_name, build_expression(idx_assignment_expression)
      );
    }
    // not exist initializer
    else
    {
      return spt::VarDef::create(ptr_type, identifier_name);
    }
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param ptr_type_declaration_declarator
 * @param idx_declarator
 * @return spt::Type*
 */
spt::Type *spt::Tree::build_pointer(
  spt::Type *ptr_type_declaration_declarator,
  ast::idx   idx_declarator
)
{
  for (auto i = ast[idx_declarator].value.declarator.is_ptr; i > 0; --i)
  {
    ptr_type_declaration_declarator
      = spt::PointerType::get(ptr_type_declaration_declarator);
  }
  return ptr_type_declaration_declarator;
}


/**
 * @brief Get the identifier name
 *
 * @param idx_declarator
 * @return std::string
 */
std::string spt::Tree::get_identifier_name(ast::idx idx_declarator)
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
 * @return spt::ArrayType*
 */
spt::ArrayType *
spt::Tree::build_array(spt::Type *ptr_unit_type, ast::idx idx_array_declarator)
{
  ast::idx idx_next_array_declarator
    = ast[idx_array_declarator].value.array_declarator.idx_next_array_declarator;
  std::vector<std::size_t> dimension_len;
  for (ast::idx i = idx_next_array_declarator; i != ast::null;
       i          = ast[i].value.array_declarator.idx_next_array_declarator)
  {
    ast::idx idx_constant = ast[idx_array_declarator].value.array_declarator.idx_constant;
    dimension_len.push_back(constant_node_to_uint64(idx_constant));
  }
  return spt::ArrayType::get(ptr_unit_type, dimension_len);
}

/**
 * @brief
 *
 * @param idx_constant
 * @return std::uint64_t
 */
std::uint64_t spt::Tree::constant_node_to_uint64(ast::idx idx_constant)
{
  return std::stoull(ast.get_constant(idx_constant).value);
}

/**
 * @brief
 *
 * @param idx_compound_statement
 * @return spt::Block*
 */
spt::Block *spt::Tree::build_block(ast::idx idx_compound_statement)
{
  spt::Block *block = spt::Block::create();

  ast::idx idx_block = ast[idx_compound_statement].value.compound_statement.idx_block;

  for (ast::idx i = idx_block; i != ast::null; i = ast[i].value.block.idx_next_block)
  {
    ast::idx idx_declaration = ast[idx_block].value.block.idx_declaration;
    if (idx_declaration != ast::null)
    {
      block->push_back(build_multi_declaration_or_definition(idx_declaration));
    }
    else
    {
      block->push_back(build_statement(i));
    }
  }
  return block;
}

/**
 * @brief
 *
 * @param idx_idx_block
 * @return spt::Block*
 */
spt::Statement *spt::Tree::build_statement(ast::idx idx_block)
{
  ast::idx idx_statement = ast[idx_block].value.block.idx_statement;
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
 * @return spt::Expr*
 */
spt::Expr *spt::Tree::build_expression(ast::idx idx_expression)
{
  switch (ast[idx_expression].type)
  {
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

spt::Expr *spt::Tree::build_binary_expression(ast::idx idx_binary_expression)
{
  return Expr::create(
    ast[idx_binary_expression].value.binary_expression.token_operator,
    build_expression(ast[idx_binary_expression].value.binary_expression.idx_left_node),
    build_expression(ast[idx_binary_expression].value.binary_expression.idx_right_node)
  );
}

spt::Expr *spt::Tree::build_assign_expression(ast::idx idx_assign_expression)
{
  ast::idx idx_binary_expression
    = ast[idx_assign_expression].value.assignment_expression.idx_binary_expression;

  // just a binary expression without assign
  spt::Expr *r_value = build_expression(idx_binary_expression);
  spt::Expr *l_value;

  for (ast::idx i = idx_assign_expression; i != ast::null;
       i          = ast[i].value.assignment_expression.idx_next_assignment_expression)
  {
    l_value = build_unary_expression(
      ast[i].value.assignment_expression.idx_unary_or_binary_expression
    );
    spt::Expr::create(
      ast[i].value.assignment_expression.assignment_type, l_value, r_value
    );

    if (ast[i].value.assignment_expression.idx_next_assignment_expression != ast::null)
    {
      r_value = l_value;
    }
  }
  return r_value;
}


spt::Expr *spt::Tree::build_unary_expression(ast::idx idx_unary_expression)
{
  //----------------------------------------------------------------
  // 1. sizeof
  //----------------------------------------------------------------
  if (ast[idx_unary_expression].value.unary_expression.is_sizeof)
  {
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
    return build_unary_expression(idx_unary_expression);
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
  if (ast[idx_unary_expression].value.unary_expression.idx_declaration_declarator)
  {
    TODO
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_conditional_expression
 * @return Expr*
 */
spt::Expr *spt::Tree::build_conditional_expression(ast::idx idx_conditional_expression
){TODO}


/**
 * @brief
 *
 * @param idx_postfix_expression
 * @return Expr*
 */
spt::Expr *spt::Tree::build_postfix_expression(ast::idx idx_postfix_expression)
{
  ast::idx idx_primary_expression
    = ast[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //--------------------------------
  // primary expression
  //--------------------------------

  //------------
  // identifier : array func call and other with identifier
  //------------
  if (ast[idx_primary_expression].value.primary_expression.idx_identifier != ast::null)
  {
    // TODO
    ast::idx idx_identifier
      = ast[idx_primary_expression].value.primary_expression.idx_identifier;
    std::string identifier = std::string(ast[idx_identifier].value.identifier.name);

    // CASE exist postfix operator
    if (ast[idx_postfix_expression].value.postfix_expression.idx_postfix_operator != ast::null)
    {
      ast::idx idx_postfix_operator
        = ast[idx_postfix_expression].value.postfix_expression.idx_postfix_operator;
      while (idx_postfix_operator != ast::null)
      {
        switch (ast[idx_postfix_operator].value.postfix_operator.postfix_operator)
        {
          // ++
          case token::self_plus:
            TODO break;

          // --
          case token::self_minus:
            TODO break;

          // ->
          case token::ver:
            TODO break;

          // .
          case token::period:
            TODO break;

          // CASE array [] or function call ()
          case token::invalid: {
            // CASE array
            if (ast[idx_postfix_operator].value.postfix_operator.idx_array_idx_assignment_expression != ast::null)
            {
              std::vector<Expr *> idx_list;
              for (ast::idx i = idx_postfix_operator; i != ast::null;
                   i          = ast[i].value.postfix_operator.idx_next_postfix_operator)
              {
                idx_list.push_back(build_expression(
                  ast[i].value.postfix_operator.idx_array_idx_assignment_expression
                ));
              }
              // TODO
              // return Array::create(identifier, idx_list);
            }
            // CASE function call
            else if (ast[idx_postfix_operator].value.postfix_operator.idx_func_call_assignment_expression_list != ast::null)
            {
              // TODO
              // return FuncCall::create(
              //   identifier,
              //   build_argument_list(
              //     ast[idx_postfix_operator]
              //       .value.postfix_operator.idx_func_call_assignment_expression_list
              //   )
              //);
            }
            else
            {
              NOT_REACHABLE
            }
            NOT_REACHABLE
          }

          default:
            SWITCH_ERROR
        }
        idx_postfix_operator
          = ast[idx_postfix_operator].value.postfix_operator.idx_next_postfix_operator;
      }
    }
    // CASE not exist postfix operator
    else
    {
      // TODO
      //  return Var::create(identifier);
    }
  }
  //------------
  // constant
  //------------
  else if (ast[idx_primary_expression].value.primary_expression.idx_constant != ast::null)
  {
    // return build_constant(
    //   ast[idx_primary_expression].value.primary_expression.idx_constant
    //);
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
 * @param idx_arguments_type_list
 * @return std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
 */
std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
spt::Tree::build_arguments_type_list(ast::idx idx_arguments_type_list)
{
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> arguments_type_list;
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
 * @return std::tuple<spt::Type *, std::string>
 */
std::tuple<spt::Type *, std::string>
spt::Tree::build_argument_declaration(ast::idx idx_argument_declaration)
{
  ast::idx idx_declaration_declarator
    = ast[idx_argument_declaration]
        .value.arguments_declaration.idx_declaration_declarator;

  ast::idx idx_declarator
    = ast[idx_argument_declaration].value.arguments_declaration.idx_declarator;

  spt::Type *ptr_type_argument = build_type(idx_declaration_declarator, idx_declarator);

  return {ptr_type_argument, get_identifier_name(idx_declarator)};
}


/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @param idx_declarator
 * @return spt::Type*
 */

spt::Type *
spt::Tree::build_type(ast::idx idx_declaration_declarator, ast::idx idx_declarator)
{
  spt::Type *ptr_type_declaration_declarator
    = build_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  spt::Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

  ast::idx idx_direct_declarator
    = ast[idx_declarator].value.declarator.idx_direct_declarator;

  // array or a pointer to array
  if (ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null)
  {
    spt::Type *ptr_unit_type = ptr_type;
    ast::idx   idx_array_declarator
      = ast[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    spt::Type *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

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
    spt::Type *ptr_return_type = ptr_type;
    // get function type list
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list
      = build_arguments_type_list(
        ast[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
      );

    spt::FuncType *ptr_func_type
      = spt::FuncType::get(ptr_return_type, std::get<0>(argument_type_list));
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
 * @return std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
 */
std::tuple<std::vector<spt::Type *>, std::vector<std::string>>
spt::Tree::build_struct_element_type_list(ast::idx idx_compound_statement){TODO}

/**
 * @brief
 *
 * @param idx_if_statement
 */
spt::Statement *spt::Tree::build_if_statement(ast::idx idx_if_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_if_statement].value.if_statement.idx_assign_expression;
  ast::idx idx_else_body = ast[idx_if_statement].value.if_statement.idx_else_body;
  ast::idx idx_if_body   = ast[idx_if_statement].value.if_statement.idx_if_body;

  // loop conditional
  spt::Expr *expr = build_expression(idx_assign_expression);
  // if body
  spt::Block *true_block = build_block(idx_if_body);
  // else body
  if (idx_else_body != ast::null)
  {
    return IfStatement::create(expr, true_block);
  }
  else
  {
    spt::Block *false_block = build_block(idx_else_body);
    return IfStatement::create(expr, true_block, false_block);
  }
}

/**
 * @brief
 *
 * @param idx_statement
 * @return spt::Statement*
 */
spt::Statement *spt::Tree::build_switch_statement(ast::idx idx_statement){TODO}

/**
 * @brief
 *
 * @param idx_statement
 * @return spt::Statement*
 */
spt::Statement *spt::Tree::build_return_statement(ast::idx idx_return_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_return_statement].value.return_statement.idx_assignment_expression;

  return ReturnStatement::create(build_expression(idx_assign_expression));
}

/**
 * @brief
 *
 * @param idx_statement
 * @return spt::Statement*
 */
spt::Statement *spt::Tree::build_while_statement(ast::idx idx_while_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_while_statement].value.while_statement.idx_assignment_expression;
  ast::idx idx_compound_statement
    = ast[idx_while_statement].value.while_statement.idx_compound_statement;

  return spt::WhileStatement::create(
    build_expression(idx_assign_expression), build_block(idx_compound_statement)
  );
}

/**
 * @brief
 *
 * @param idx_statement
 * @return spt::Statement*
 */
spt::Statement *spt::Tree::build_do_while_statement(ast::idx idx_do_while_statement)
{
  ast::idx idx_assign_expression
    = ast[idx_do_while_statement].value.do_while_statement.idx_assign_statement;
  ast::idx idx_compound_statement
    = ast[idx_do_while_statement].value.do_while_statement.idx_compound_statement;

  return spt::DoWhileStatement::create(
    build_expression(idx_assign_expression), build_block(idx_compound_statement)
  );
}

/**
 * @brief
 *
 * @param idx_statement
 * @return spt::Statement*
 */
spt::Statement *spt::Tree::build_for_statement(ast::idx idx_for_statement)
{
  ast::idx idx_change_assign_expression
    = ast[idx_for_statement].value.for_statement.idx_change_assign_expression;

  ast::idx idx_compound_statement
    = ast[idx_for_statement].value.for_statement.idx_compound_statement;

  ast::idx idx_conditional_assign_expression
    = ast[idx_for_statement].value.for_statement.idx_conditional_assign_expression;

  ast::idx idx_declaration = ast[idx_for_statement].value.for_statement.idx_declaration;

  // declarations
  build_multi_declaration_or_definition(idx_declaration);
  // condition
  build_expression(idx_conditional_assign_expression);
  // change value
  build_expression(idx_change_assign_expression);
  // compound statement
  build_block(idx_compound_statement);
  TODO
}

spt::Expr *spt::Tree::build_compute_unary_expression_size(ast::idx unary_expression){TODO}

spt::Expr *spt::Tree::build_compute_declarator_size(ast::idx idx_declaration_declarator)
{
  TODO
}

}  // namespace spt

#endif