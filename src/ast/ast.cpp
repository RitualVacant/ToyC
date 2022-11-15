#ifndef syntax_TREE_CPP
#define syntax_TREE_CPP

#include "ast.h"
#include "declataror.h"
#include "global_var.h"
#include "inner.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <stdlib.h>
#include <string>

namespace ast
{

Tree::Tree()
{
  if (reserve_tree)
  {
    tree_body.reserve(sizeof_syntax_tree_init);
  }
  tree_body.push_back({});
  constant_node_tree.push_back({});
}

Tree::~Tree() {}


ast::Tree &Tree::operator=(const ast::Tree &&tree_)
{
  tree_body          = std::move(tree_.tree_body);
  constant_node_tree = std::move(tree_.constant_node_tree);
  return *this;
}


ast::node &Tree::operator[](ast::idx idx)
{
  return tree_body[idx];
}

ast::constant_node &Tree::get_constant(ast::idx idx_constant)
{
  return constant_node_tree[idx_constant];
}

ast::idx Tree::create_constant_node(ast::node_type node_type, std::string value)
{
  constant_node_tree.push_back(ast::constant_node{node_type, value});
  return constant_node_tree.size() - 1;
}

ast::idx Tree::creat_node(ast::node_type node_type)
{
  tree_body.push_back({});
  tree_body.back().type = node_type;
  return tree_body.size() - 1;
}

// DROP
void Tree::trans_tree()
{
  print_trans_tree = true;
  for (ast::idx i = 1; i != ast::null;
       i
       = tree_body[i].value.declaration_or_definition.idx_next_declaration_or_definition)
  {
    trans_declaration_or_definition(i);
  }
}

// DROP
void Tree::trans_declaration_or_definition(ast::idx idx_declaration_or_definition)
{
  ast::idx idx_start_initial_declarator
    = tree_body[idx_declaration_or_definition]
        .value.declaration_or_definition.idx_initial_declarator;

  idx_now_declaration_declarator
    = tree_body[idx_declaration_or_definition]
        .value.declaration_or_definition.idx_declaration_declarator;

  idx_now_compound_statement = tree_body[idx_declaration_or_definition]
                                 .value.declaration_or_definition.idx_compound_statement;

  // trans will cover the next idx value in declarator
  // j store next idx before next idx been covered
  for (ast::idx i = idx_start_initial_declarator,
                j = tree_body[i].value.initial_declarator.idx_next_initial_declarator;
       i != ast::null; i = j)
  {
    trans_each_initial_declarator(i);
  }
}

// DROP
void Tree::trans_each_initial_declarator(ast::idx idx_initial_declarator)
{
  switch (which_type(idx_initial_declarator))
  {
    case type_decl_defi::array_decl:
      tree_body[idx_initial_declarator].type = ast::node_type::array_definition;
      trans_to_arrary_definition(idx_initial_declarator);
      break;
    case type_decl_defi::enum_defi:
      tree_body[idx_initial_declarator].type = ast::node_type::enum_definition;
      trans_to_enum_definition(idx_initial_declarator);
      break;
    case type_decl_defi::func_decl:
    case type_decl_defi::func_defi:
      trans_to_function(idx_initial_declarator);
      break;
    case type_decl_defi::struct_decl:
      trans_to_struct_declaration(idx_initial_declarator);
      break;
    case type_decl_defi::struct_defi:
      trans_to_struct_definition(idx_initial_declarator);
      break;
    case type_decl_defi::var_decl:
      tree_body[idx_initial_declarator].type = ast::node_type::basic_type_declaration;
      trans_to_basic_type_declaration(idx_initial_declarator);
      break;
    default:
      SWITCH_ERROR
  }
  NOT_REACHABLE
}

Tree::type_decl_defi Tree::which_type(ast::idx idx_initial_declarator)
{
  ast::idx idx_direct_declarator
    = tree_body[tree_body[idx_initial_declarator].value.initial_declarator.idx_declarator]
        .value.declarator.idx_direct_declarator;

  // function
  if (tree_body[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null)
  {
    if (tree_body[idx_initial_declarator].value.declaration_or_definition.idx_compound_statement != ast::null)
    {
      return type_decl_defi::func_defi;
    }
    else
    {
      return type_decl_defi::func_decl;
    }
  }

  // array
  if (tree_body[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null)
  {
    return type_decl_defi::array_decl;
  }

  // struct
  if (tree_body[idx_now_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_struct)
  {
    if (idx_now_compound_statement != ast::null)
    {
      return type_decl_defi::struct_defi;
    }
    else
    {
      return type_decl_defi::struct_decl;
    }
  }

  // enum
  if (tree_body[idx_now_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_enum)
  {
    return type_decl_defi::enum_defi;
  }

  return var_decl;
}

void Tree::trans_to_function(ast::idx idx)
{
  tree_body[idx].type = ast::node_type::function;

  ast::idx idx_next_declarator
    = tree_body[idx].value.initial_declarator.idx_next_initial_declarator;
  ast::idx idx_declarator = tree_body[idx].value.initial_declarator.idx_declarator;
  ast::idx idx_identifier;

  // because the function pointer
  for (ast::idx i = idx_declarator; tree_body[i].type != ast::node_type::identifier;)
  {
    switch (tree_body[i].type)
    {
      case ast::node_type::declarator:
        i = tree_body[i].value.declarator.idx_direct_declarator;
        break;
      case ast::node_type::direct_declarator:
        if (tree_body[i].value.direct_declarator.idx_identifier != ast::null)
        {
          i              = tree_body[i].value.direct_declarator.idx_identifier;
          idx_identifier = i;
        }
        else
        {
          i = tree_body[i].value.direct_declarator.idx_declarator;
        }
        break;
      default:
        fmt::print(
          fg(fmt::color::red), "node type {}\n",
          static_cast<unsigned char>(tree_body[i].type)
        );
        SWITCH_ERROR
    }
  }

  ast::idx idx_arguments_type_list
    = tree_body[tree_body[tree_body[idx].value.initial_declarator.idx_declarator]
                  .value.declarator.idx_direct_declarator]
        .value.direct_declarator.idx_arguments_type_list;

  //
  // cover
  //

  // function name
  tree_body[idx].value.function_declaration.idx_function_name = idx_identifier;

  // function type and return type
  tree_body[idx].value.function_declaration.idx_function_declarator = idx_declarator;

  // return type
  tree_body[idx].value.function_declaration.idx_function_return_type
    = idx_now_declaration_declarator;

  // function body
  tree_body[idx].value.function_declaration.idx_function_body
    = idx_now_compound_statement;

  // next declaration
  tree_body[idx].value.function_declaration.idx_next = idx_next_declarator;

  // arguments list
  tree_body[idx].value.function_declaration.idx_function_arguments_type_list
    = idx_arguments_type_list;
  return;
}

void Tree::trans_to_arrary_definition(ast::idx idx_initial_declarator)
{
  tree_body[idx_initial_declarator].type = ast::node_type::struct_declaration;

  // type of struct
  tree_body[idx_initial_declarator].value.struct_definition.idx_struct_type
    = idx_now_declaration_declarator;

  // struct identifer
  tree_body[idx_initial_declarator].value.struct_definition.idx_struct_name
    = tree_body[tree_body[tree_body[idx_initial_declarator]
                            .value.initial_declarator.idx_declarator]
                  .value.declarator.idx_direct_declarator]
        .value.direct_declarator.idx_identifier;

  return;
}
void Tree::trans_to_struct_definition(ast::idx idx_initial_declarator)
{
  tree_body[idx_initial_declarator].type = ast::node_type::struct_definition;

  // type of struct
  tree_body[idx_initial_declarator].value.struct_definition.idx_struct_type
    = idx_now_declaration_declarator;

  // struct body
  tree_body[idx_initial_declarator].value.struct_definition.idx_struct_body
    = idx_now_compound_statement;

  // struct identifier
  tree_body[idx_initial_declarator].value.struct_definition.idx_struct_name
    = tree_body[tree_body[tree_body[idx_initial_declarator]
                            .value.initial_declarator.idx_declarator]
                  .value.declarator.idx_direct_declarator]
        .value.direct_declarator.idx_identifier;

  return;
}
void Tree::trans_to_struct_declaration(ast::idx idx_initial_declarator)
{
  tree_body[idx_initial_declarator].type = ast::node_type::struct_declaration;
  return;
}
void Tree::trans_to_basic_type_declaration(ast::idx idx)
{
  return;
}
void Tree::trans_to_enum_definition(ast::idx idx)
{
  return;
}

void Tree::connect(ast::idx idx)
{
  if (last_root_ptr != ast::null)
  {
    tree_body[last_root_ptr]
      .value.declaration_or_definition.idx_next_declaration_or_definition
      = idx;
  }
  else
  {
    last_root_ptr = idx;
  }
  return;
}

void Tree::print_tree()
{
  toy_c::fstream_guard file(output_file_path, toy_c::mode::write);
  json.init();
  dfs_print_tree(1);
  json.end();
  file << json.get_str_ref();
}

void Tree::dfs_print_tree(ast::idx idx)
{
  if (idx == ast::null)
  {
    return;
  }
  std::string num(std::to_string(idx));
  switch (tree_body[idx].type)
  {
    case ast::node_type::conditional_expression:
      json.print_class("conditional_expression" + num, [&] {
        dfs_print_tree(tree_body[idx].value.conditional_expression.idx_binary_expression);
        dfs_print_tree(tree_body[idx].value.conditional_expression.idx_expression);
        dfs_print_tree(
          tree_body[idx].value.conditional_expression.idx_conditional_expression
        );
      });
      return;
    case ast::node_type::assignment_expression:
      json.print_class("assignment_expression" + num, [&] {
        json.print_key_value(
          "assignment_type", operator_token_to_symbol(
                               tree_body[idx].value.assignment_expression.assignment_type
                             )
        );
        dfs_print_tree(tree_body[idx].value.assignment_expression.idx_binary_expression);
        dfs_print_tree(
          tree_body[idx].value.assignment_expression.idx_unary_or_binary_expression
        );
        json.print_class("next_assign", [&] {
          dfs_print_tree(
            tree_body[idx].value.assignment_expression.idx_next_assignment_expression
          );
        });
      });
      return;

    case ast::node_type::unary_expression:
      json.print_class("unary_expression" + num, [&] {
        dfs_print_tree(tree_body[idx].value.unary_expression.idx_declaration_declarator);
        dfs_print_tree(tree_body[idx].value.unary_expression.idx_postfix_expression);
        dfs_print_tree(tree_body[idx].value.unary_expression.idx_unary_expression);
        if (tree_body[idx].value.unary_expression.is_sizeof)
        {
          json.print_key_value("is_sizeof", "true");
        }
        else
        {
          json.print_key_value("is_sizeof", "false");
        }
        json.print_key_value(
          "unary_operator",
          operator_token_to_symbol(tree_body[idx].value.unary_expression.unary_operator)
        );
      });
      return;

    case ast::node_type::while_statement:
      json.print_class("while_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.while_statement.idx_assignment_expression);
        dfs_print_tree(tree_body[idx].value.while_statement.idx_compound_statement);
      });
      return;

    case ast::node_type::postfix_expression:
      json.print_class("postfix_expression" + num, [&] {
        dfs_print_tree(tree_body[idx].value.postfix_expression.idx_postfix_operator);
        dfs_print_tree(tree_body[idx].value.postfix_expression.idx_primary_expression);
      });
      return;

    case ast::node_type::primary_expression:
      json.print_class("primary_expression" + num, [&] {
        ast::idx idx_constant
          = tree_body[idx].value.primary_expression.idx_constant != ast::null;
        if (idx_constant != ast::null)
        {
          print_json_constant(idx_constant);
        }
        else
        {
          dfs_print_tree(tree_body[idx].value.primary_expression.idx_identifier);
          dfs_print_tree(tree_body[idx].value.primary_expression.idx_expression);
        }
      });
      return;

    case ast::node_type::binary_expression:
      json.print_class("binary_expression" + num, [&] {
        json.print_key_value(
          "operator",
          operator_token_to_symbol(tree_body[idx].value.binary_expression.token_operator)
        );
        dfs_print_tree(tree_body[idx].value.binary_expression.idx_left_node);
        dfs_print_tree(tree_body[idx].value.binary_expression.idx_right_node);
      });
      return;

    case ast::node_type::expression:
      json.print_class("expression" + num, [&] {
        dfs_print_tree(tree_body[idx].value.expression.idx_assignment_expression);
      });
      dfs_print_tree(tree_body[idx].value.expression.idx_next_expression);
      return;

    case ast::node_type::initializer:
      json.print_class("initializer" + num, [&] {
        dfs_print_tree(tree_body[idx].value.initializer.idx_assignment_expression);
        dfs_print_tree(tree_body[idx].value.initializer.idx_initializer_list);
      });
      return;

    case ast::node_type::initializer_list:
      for (ast::idx i = idx; i != ast::null;
           i          = tree_body[i].value.initializer_list.idx_next_initializer_list)
      {
        json.print_class("initializer_list" + std::to_string(i), [&] {
          dfs_print_tree(
            tree_body[i].value.initializer_list.idx_head_initializer_list_node
          );
          dfs_print_tree(tree_body[i].value.initializer_list.idx_son_initializer_list);
        });
      }
      return;

    case ast::node_type::initializer_list_node:
      for (ast::idx i = idx; i != ast::null;
           i = tree_body[i].value.initializer_list_node.idx_next_initializer_list_node)
      {
        json.print_class("initializer_list_node" + num, [&] {
          json.print_key_value(
            "constant",
            constant_node_tree[tree_body[i].value.initializer_list_node.idx_constant]
              .value
          );
        });
      }
      return;

    case ast::node_type::declare_function:
      json.print_class("declare_function" + num, [&] {
        dfs_print_tree(tree_body[idx].value.declare_function.idx_function_arguments);
        dfs_print_tree(tree_body[idx].value.declare_function.idx_return_value_type);
      });
      return;

    case ast::node_type::definition_function:
      json.print_class("definition_function" + num, [&] {
        dfs_print_tree(tree_body[idx].value.definition_function.idx_function_arguments);
        dfs_print_tree(tree_body[idx].value.definition_function.idx_return_value_type);
        dfs_print_tree(tree_body[idx].value.definition_function.idx_compound_statement);
      });
      return;

    case ast::node_type::initial_declarator_list:
      json.print_class("initial_declarator_list" + num, [&] {
        dfs_print_tree(tree_body[idx].value.initial_declarator_list.idx_initial_declarator
        );
      });
      return;

    case ast::node_type::definition_struct:
      TODO break;

    case ast::node_type::constant:
      json.print_class("constant" + num, [&] {
        json.print_key_value("value", tree_body[idx].value.constant.const_value);
      });
      return;

    case ast::node_type::operators: {
      NOT_REACHABLE
    }
      return;

    case ast::node_type::initial_declarator:
      json.print_class("initial_declarator" + num, [&] {
        dfs_print_tree(tree_body[idx].value.initial_declarator.idx_declarator);
        dfs_print_tree(tree_body[idx].value.initial_declarator.idx_initializer);
        dfs_print_tree(tree_body[idx].value.initial_declarator.idx_next_initial_declarator
        );
      });
      return;

    case ast::node_type::direct_declarator:
      json.print_class("direct_declarator" + num, [&] {
        dfs_print_tree(tree_body[idx].value.direct_declarator.idx_identifier);
        dfs_print_tree(tree_body[idx].value.direct_declarator.idx_declarator);
        dfs_print_tree(tree_body[idx].value.direct_declarator.idx_array_declarator);
        if (!print_trans_tree)
        {
          dfs_print_tree(tree_body[idx].value.direct_declarator.idx_arguments_type_list);
        }
      });
      return;

    case ast::node_type::array_declarator:
      json.print_class("array_declarator" + num, [&] {
        dfs_print_tree(tree_body[idx].value.array_declarator.idx_constant);
        dfs_print_tree(tree_body[idx].value.array_declarator.idx_next_array_declarator);
      });
      return;

    case ast::node_type::declarator:
      json.print_class("declarator" + num, [&] {
        json.print_key_value(
          "is_pointer", std::to_string(tree_body[idx].value.declarator.is_ptr)
        );
        dfs_print_tree(tree_body[idx].value.declarator.idx_direct_declarator);
      });
      return;

    case ast::node_type::declaration_or_definition:
      json.print_class("declaration_or_definition" + num, [&] {
        if (!print_trans_tree)
        {
          dfs_print_tree(
            tree_body[idx].value.declaration_or_definition.idx_declaration_declarator
          );
          dfs_print_tree(
            tree_body[idx].value.declaration_or_definition.idx_compound_statement
          );
        }
        dfs_print_tree(
          tree_body[idx].value.declaration_or_definition.idx_initial_declarator
        );
        dfs_print_tree(
          tree_body[idx]
            .value.declaration_or_definition.idx_next_declaration_or_definition
        );
      });
      return;

    case ast::node_type::identifier:
      json.print_class("identifier" + num, [&] {
        json.print_key_value("name", tree_body[idx].value.identifier.name);
      });
      return;

    case ast::node_type::arguments_type_list:
      json.print_class("arguments_type_list" + num, [&] {
        dfs_print_tree(tree_body[idx].value.arguments_type_list.idx_argument_declaration);
      });
      return;

    case ast::node_type::arguments_declaration:
      json.print_class("arguments_declaration" + num, [&] {
        dfs_print_tree(
          tree_body[idx].value.arguments_declaration.idx_declaration_declarator
        );
        dfs_print_tree(tree_body[idx].value.arguments_declaration.idx_declarator);
      });
      dfs_print_tree(
        tree_body[idx].value.arguments_declaration.idx_next_arguments_declaration
      );
      return;

    case ast::node_type::compound_statement:
      json.print_class("compound_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.compound_statement.idx_block);
      });
      return;

    case ast::node_type::block:
      json.print_class("block" + num, [&] {
        dfs_print_tree(tree_body[idx].value.block.idx_declaration);
        dfs_print_tree(tree_body[idx].value.block.idx_statement);
      });
      dfs_print_tree(tree_body[idx].value.block.idx_next_block);
      return;

    case ast::node_type::postfix_operator:
      json.print_class("postfix_operator" + num, [&] {
        switch (tree_body[idx].value.postfix_operator.postfix_operator)
        {
          case token::ver:
            print_key_value("postfix_operator", "->");
            return;
          case token::period:
            print_key_value("postfix_operator", ".");
            return;
          case token::self_plus:
            print_key_value("postfix_operator", "++");
            return;
          case token::self_minus:
            print_key_value("postfix_operator", "--");
            return;
          case token::invalid:
            break;
          default:
            PRINT_TOKEN(tree_body[idx].value.postfix_operator.postfix_operator)
            SWITCH_ERROR
        }
        dfs_print_tree(
          tree_body[idx].value.postfix_operator.idx_array_idx_assignment_expression
        );
        dfs_print_tree(tree_body[idx].value.postfix_operator.idx_identifier);
        dfs_print_tree(
          tree_body[idx].value.postfix_operator.idx_func_call_assignment_expression_list
        );
        dfs_print_tree(tree_body[idx].value.postfix_operator.idx_next_postfix_operator);
      });
      return;

    case ast::node_type::if_statement:
      json.print_class("if_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.if_statement.idx_assign_expression);
        dfs_print_tree(tree_body[idx].value.if_statement.idx_if_body);
        dfs_print_tree(tree_body[idx].value.if_statement.idx_else_body);
      });
      return;

    case ast::node_type::for_statement:
      json.print_class("for_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.for_statement.idx_declaration);
        dfs_print_tree(
          tree_body[idx].value.for_statement.idx_conditional_assign_expression
        );
        dfs_print_tree(tree_body[idx].value.for_statement.idx_change_assign_expression);
        dfs_print_tree(tree_body[idx].value.for_statement.idx_compound_statement);
      });
      return;

    case ast::node_type::switch_statement:
      json.print_class("switch_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.switch_statement.idx_assign_expression);
        dfs_print_tree(tree_body[idx].value.switch_statement.idx_compound_statement);
      });
      return;

    case ast::node_type::do_while_statement:
      json.print_class("do_while_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.do_while_statement.idx_assign_statement);
        dfs_print_tree(tree_body[idx].value.do_while_statement.idx_compound_statement);
      });
      return;

    case ast::node_type::continue_statement:
      json.print_class("continue_statement" + num, [] {});
      return;

    case ast::node_type::break_statement:
      json.print_class("break_statement" + num, [] {});
      return;

    case ast::node_type::default_label:
      json.print_class("default_label" + num, [] {});
      return;

    case ast::node_type::case_label:
      json.print_class("case_label" + num, [&] {
        dfs_print_tree(tree_body[idx].value.case_label.const_expression);
      });
      return;

    // TODO
    case ast::node_type::basic_type_declaration:
      json.print_class("basic_type_declaration" + num, [&] {
        dfs_print_tree(tree_body[idx].value.basic_type_declaration.idx_next);
      });
      return;

    case ast::node_type::struct_declaration:
      json.print_class("struct_declaration" + num, [&] {
        dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_type);
        dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_name);
      });
      return;

    case ast::node_type::struct_definition:
      json.print_class("struct_definition" + num, [&] {
        dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_type);
        dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_name);
        dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_body);
      });
      return;

    case ast::node_type::declaration_declarator:
      json.print_class("declaration_declarator" + num, [&] {
        auto &decl = tree_body[idx].value.declaration_declarator;
        if (decl.sign == ast::declarator_sign::sign_signed)
        {
          json.print_key_value("sign", "true");
        }
        else
        {
          json.print_key_value("sign", "false");
        }

        switch (decl.type)
        {
          case ast::declarator_type::type_char:
            json.print_key_value("type", "char");
            break;
          case ast::declarator_type::type_double:
            json.print_key_value("type", "double");
            break;
          case ast::declarator_type::type_float:
            json.print_key_value("type", "float");
            break;
          case ast::declarator_type::type_int:
            json.print_key_value("type", "int");
            break;
          case ast::declarator_type::type_long_int:
            json.print_key_value("type", "long_int");
            break;
          case ast::declarator_type::type_long_long_int:
            json.print_key_value("type", "long_long_int");
            break;
          case ast::declarator_type::type_short_int:
            json.print_key_value("type", "short_int");
            break;
          case ast::declarator_type::type_void:
            json.print_key_value("type", "void");
            break;
          case ast::declarator_type::type_struct:
            json.print_key_value("type", "struct");
            break;
          default:
            SWITCH_ERROR
        }

        if (decl.limit == ast::declarator_limit::limit_Alignas)
        {
          json.print_key_value("limit", "Alignas");
        }
        else
        {
          json.print_key_value("limit", "empty");
        }

        switch (decl.store)
        {
          case ast::declarator_store::store_empty:
            json.print_key_value("store", "empty");
            break;
          case ast::declarator_store::store_extern:
            json.print_key_value("store", "extern");
            break;
          case ast::declarator_store::store_static:
            json.print_key_value("store", "static");
            break;
          case ast::declarator_store::store_typedef:
            json.print_key_value("store", "typedef");
            break;
          default:
            break;
        }
        dfs_print_tree(
          tree_body[idx].value.declaration_declarator.idx_struct_union_identifier
        );
      });
      return;

    // second step node
    case ast::node_type::function:
      json.print_class("function_declaration" + num, [&] {
        dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_name);
        dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_declarator);
        dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_return_type
        );
        dfs_print_tree(
          tree_body[idx].value.function_declaration.idx_function_arguments_type_list
        );
        dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_body);
        dfs_print_tree(tree_body[idx].value.function_declaration.idx_next);
      });
      return;

    case ast::node_type::return_statement:
      json.print_class("return_statement" + num, [&] {
        dfs_print_tree(tree_body[idx].value.return_statement.idx_assignment_expression);
      });
      return;

    case ast::node_type::arguments_list_node:
      json.print_class("arguments_list_node" + num, [&] {
        for (ast::idx i = idx; i != ast::null;
             i = tree_body[i].value.arguments_list_node.idx_next_arguments_list_node)
        {
          dfs_print_tree(
            tree_body[idx].value.arguments_list_node.idx_assignment_expression
          );
        }
      });
      return;

    default: {
      fmt::print(
        fg(fmt::color::red), "add node type : {}\n",
        static_cast<unsigned char>(tree_body[idx].type)
      );
      SWITCH_ERROR
    }
  }
  return;
}

void Tree::print_key_value(std::string key, std::string value)
{
  file_buffer += fmt::format("\"{}\":\"{}\",", key, value);
  return;
}

void Tree::print_class_head(std::string value)
{
  file_buffer += "\"";
  file_buffer += value;
  file_buffer += '_';
  file_buffer += std::to_string(now_idx);
  file_buffer += "\":{";
  return;
}

void Tree::print_class_end()
{
  file_buffer += "},";
  return;
}

void Tree::print_json_key(std::string key)
{
  file_buffer += "\"";
  file_buffer += key;
  file_buffer += "\":";
  return;
}

void Tree::print_json_value(std::string value)
{
  file_buffer += "\"";
  file_buffer += value;
  file_buffer += "\",";
  return;
}

void Tree::print_json_constant(ast::idx idx_constant)
{
  switch (constant_node_tree[idx_constant].type)
  {
    case ast::node_type::constant_integer_number:
      file_buffer += fmt::format(
        "\"{}_{}\":\"{}\",", "constant_integer_number", idx_constant,
        constant_node_tree[idx_constant].value
      );
      break;
    case ast::node_type::constant_float_number:
      file_buffer += fmt::format(
        "\"{}_{}\":\"{}\",", "constant_float_number", idx_constant,
        constant_node_tree[idx_constant].value
      );
      break;
    case ast::node_type::constant_negative_float_number:
      file_buffer += fmt::format(
        "\"{}_{}\":\"{}\",", "constant_negative_float_number", idx_constant,
        constant_node_tree[idx_constant].value
      );
      break;
    case ast::node_type::constant_negative_integer_number:
      file_buffer += fmt::format(
        "\"{}_{}\":\"{}\",", "constant_negative_integer_number", idx_constant,
        constant_node_tree[idx_constant].value
      );
      break;
    default:
      PRINT_NODE_TYPE(constant_node_tree[idx_constant].type)
      SWITCH_ERROR
  }
}

}  // namespace ast

#endif