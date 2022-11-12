#ifndef syntax_TREE_CPP
#define syntax_TREE_CPP

#include "ast.h"
#include "declataror.h"
#include "global_var.h"
#include "inner.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <stdlib.h>

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

// int done = system("python3.8 -m json.tool
// /home/lzj/code/program/script/test/Tree.json
// /home/lzj/code/program/script/test/tree1.json"); if (done == 127) {
//     fmt::print("output json Tree");
// }
// if (done == -1) {
//     fmt::print("output json Tree");
// }

ast::Tree &Tree::operator=(const ast::Tree &&tree_)
{
  tree_body          = std::move(tree_.tree_body);
  constant_node_tree = std::move(tree_.constant_node_tree);
  return *this;
}

/*
ast::idx
Tree::insert(ast::node_type node_type) {
    //type_list.push_back(node_type);
    Tree.push_back({});
    switch (node_type) {
        case ast::node_type::type: {
        }
        case ast::node_type::expression: {
        }
        case ast::node_type::assignment_expression: {

        }
        case ast::node_type::conditional_expression:
        case ast::node_type::binary_expression:
        case ast::node_type::unary_expression:
        case ast::node_type::postfix_expression:
        case ast::node_type::primary_expression:
        case ast::node_type::statement:
        case ast::node_type::declare_function: {
            //Tree.back().value.declare_function.loc.line = line;
            //Tree.back().value.declare_function.loc.column = column;

        }
        case ast::node_type::declare_variable: {

        }
        case ast::node_type::definition_function: {

        }
        case ast::node_type::constant:
        case ast::node_type::operators:
        default: {

        }
    }
    //++size;
    //++end;
    //return end;
}
*/

ast::node &Tree::operator[](ast::idx idx)
{
#ifdef DEBUG
  if (idx == ast::null)
  {
    fmt::print(fg(fmt::color::red), "ast::idx error : ast::idx can't be ast::null\n");
    exit(0);
  }
  if (idx >= Tree.size())
  {
    fmt::print(fg(fmt::color::red), "ast::idx error : ast::idx out of range\n");
    exit(0);
  }
#endif
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
  /*
  switch (node_type) {
      case ast::node_type::type:
          Tree.back().value.type.is_init = true;
          break;
      case ast::node_type::expression:
          Tree.back().value.expression.is_init = true
          break;
      case ast::node_type::assignment_expression:
          Tree.back().
          break;
      case ast::node_type::conditional_expression:
          Tree.back().
          break;
      case ast::node_type::binary_expression:
          Tree.back().
          break;
      case ast::node_type::unary_expression:
          Tree.back().
          break;
      case ast::node_type::postfix_expression:
          Tree.back().
          break;
      case ast::node_type::primary_expression:
          Tree.back().
          break;
      case ast::node_type::statement:
          Tree.back().
          break;
      case ast::node_type::declare_varible:
          Tree.back().
          break;
      case ast::node_type::declare_function:
          Tree.back().
          break;
      case ast::node_type::definition_function:
          Tree.back().
          break;
      case ast::node_type::definition_struct:
          Tree.back().
          break;
      case ast::node_type::constant:
          Tree.back().
          break;
      case ast::node_type::operators:
          Tree.back().
          break;
      case ast::node_type::initial_declarator:
          Tree.back().
          break;
      case ast::node_type::direct_declarator:
          Tree.back().
          break;
      case ast::node_type::declarator:
          Tree.back().
          break;
      case ast::node_type::declaration_declarator:
          Tree.back().
          break;
      case ast::node_type::initial_declarator_list:
          Tree.back().
          break;
      case ast::node_type::declaration_or_definition:
          Tree.back().
          break;
      case ast::node_type::identifier:
          Tree.back().
          break;
      case ast::node_type::arguments_declaration:
          Tree.back().
          break;
      case ast::node_type::arguments_type_list:
          Tree.back().
          break;
      case ast::node_type::compound_statement:
          Tree.back().
          break;
      case ast::node_type::block_list:
          Tree.back().
          break;
      case ast::node_type::block:
          Tree.back().
          break;
      case ast::node_type::mark_statement:
          Tree.back().
          break;
      default:
          SWITCH_ERROR
  }
  */
}


//
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

void Tree::trans_each_initial_declarator(ast::idx idx_initial_declarator)
{
  switch (which_type(idx_initial_declarator))
  {
    case type_decl_defi::array_decl:
      tree_body[idx_initial_declarator].type = ast::node_type::array_definintion;
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

  // struct idnentifer
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
  if (!file.is_open())
  {
    fmt::print("can't open file in Tree");
    exit(0);
  }

  file_buffer.clear();
  file_buffer += '{';
  dfs_print_tree(1);
  file_buffer += '}';

  // erase extra comma in json file string
  // just replace comma with /n
  for (std::size_t i = 0, j = 1; j < file_buffer.size(); ++i, ++j)
  {
    if (file_buffer[i] == ',' && file_buffer[j] == '}')
    {
      file_buffer[i] = '\n';
    }
  }

  file << file_buffer;

  return;
}

void Tree::dfs_print_tree(ast::idx idx)
{
  if (idx == ast::null)
    return;
  now_idx = idx;
  switch (tree_body[idx].type)
  {
    case ast::node_type::conditional_expression:
      print_json_class_head("conditional_expression");
      dfs_print_tree(tree_body[idx].value.conditional_expression.idx_binary_expression);
      dfs_print_tree(tree_body[idx].value.conditional_expression.idx_expression);
      dfs_print_tree(
        tree_body[idx].value.conditional_expression.idx_conditional_expression
      );
      print_json_class_end();
      break;
    case ast::node_type::assignment_expression:
      print_json_class_head("assignment_expression");
      print_json_key("assignment_type");
      switch (tree_body[idx].value.assignment_expression.assignment_type)
      {
        case token::assign:
          print_json_value("=");
          break;
        case token::plus_agn:
          print_json_value("+=");
          break;
        case token::minus_agn:
          print_json_value("-=");
          break;
        case token::times_agn:
          print_json_value("*=");
          break;
        case token::div_agn:
          print_json_value("/=");
          break;
        case token::mod_agn:
          print_json_value("%=");
          break;
        case token::r_shift_agn:
          print_json_value(">>=");
          break;
        case token::l_shift_agn:
          print_json_value("<<=");
          break;
        case token::bit_and_agn:
          print_json_value("&=");
          break;
        case token::bit_or_agn:
          print_json_value("|=");
          break;
        case token::bit_xor_agn:
          print_json_value("^=");
          break;
        default:
          SWITCH_ERROR
      }
      dfs_print_tree(tree_body[idx].value.assignment_expression.idx_binary_expression);
      dfs_print_tree(
        tree_body[idx].value.assignment_expression.idx_unary_or_binary_expression
      );
      print_json_class_head("next_assign");
      dfs_print_tree(
        tree_body[idx].value.assignment_expression.idx_next_assignment_expression
      );
      print_json_class_end();
      print_json_class_end();
      break;

    case ast::node_type::unary_expression:
      print_json_class_head("unary_expression");
      dfs_print_tree(tree_body[idx].value.unary_expression.idx_declaration_declarator);
      dfs_print_tree(tree_body[idx].value.unary_expression.idx_postfix_expression);
      dfs_print_tree(tree_body[idx].value.unary_expression.idx_unary_expression);

      print_json_key("is_sizeof");
      if (tree_body[idx].value.unary_expression.is_sizeof)
      {
        print_json_value("true");
      }
      else
      {
        print_json_value("false");
      }

      print_json_key("unary_operator");
      switch (tree_body[idx].value.unary_expression.unary_operator)
      {
        case token::bit_and:
          print_json_value("&");
          break;
        case token::log_not:
          print_json_value("!");
          break;
        case token::bit_not:
          print_json_value("~");
          break;
        case token::times:
          print_json_value("*");
          break;
        case token::invalid:
          print_json_value("void");
          break;
        case token::self_plus:
          print_json_value("++");
          break;
        case token::self_minus:
          print_json_value("--");
          break;
        default:
          std::cout << fmt::format(
            fg(fmt::color::red), "the operator is {}\n",
            static_cast<unsigned int>(tree_body[idx].value.unary_expression.unary_operator
            )
          );
          SWITCH_ERROR
          break;
      }
      print_json_class_end();
      break;

    case ast::node_type::while_statement:
      print_json_class_head("while_statement");
      dfs_print_tree(tree_body[idx].value.while_statement.idx_assignment_expression);
      dfs_print_tree(tree_body[idx].value.while_statement.idx_compound_statement);
      print_json_class_end();
      break;

    case ast::node_type::postfix_expression:
      print_json_class_head("postfix_expression");
      dfs_print_tree(tree_body[idx].value.postfix_expression.idx_postfix_operator);
      dfs_print_tree(tree_body[idx].value.postfix_expression.idx_primary_expression);
      print_json_class_end();
      break;

    case ast::node_type::primary_expression: {
      print_json_class_head("primary_expression");
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
      print_json_class_end();
      break;
    }

    case ast::node_type::binary_expression:
      print_json_class_head("binary_expression");
      print_json_key("operator");
      switch (tree_body[idx].value.binary_expression.token_operator)
      {
        case token::log_or:
          print_json_value("||");
          break;
        case token::log_and:
          print_json_value("&&");
          break;
        case token::bit_or:
          print_json_value("|");
          break;
        case token::bit_xor:
          print_json_value("^");
          break;
        case token::bit_and:
          print_json_value("&");
          break;
        case token::equ:
          print_json_value("==");
          break;
        case token::not_equ:
          print_json_value("!=");
          break;
        case token::less:
          print_json_value("<");
          break;
        case token::less_equ:
          print_json_value("<=");
          break;
        case token::great:
          print_json_value(">");
          break;
        case token::great_equ:
          print_json_value(">=");
          break;
        case token::l_shift:
          print_json_value("<<");
          break;
        case token::r_shift:
          print_json_value(">>");
          break;
        case token::plus:
          print_json_value("+");
          break;
        case token::minus:
          print_json_value("-");
          break;
        case token::div:
          print_json_value("/");
          break;
        case token::mod:
          print_json_value("%");
          break;
        case token::times:
          print_json_value("*");
          break;
        default:
          SWITCH_ERROR
      }
      dfs_print_tree(tree_body[idx].value.binary_expression.idx_left_node);
      dfs_print_tree(tree_body[idx].value.binary_expression.idx_right_node);
      print_json_class_end();
      break;

    case ast::node_type::expression:
      print_json_class_head("expression");
      dfs_print_tree(tree_body[idx].value.expression.idx_assignment_expression);
      print_json_class_end();
      dfs_print_tree(tree_body[idx].value.expression.idx_next_expression);
      break;

    /*
    case ast::node_type::type:
        print_json_class_head("type");
        print_json_class_end();
        break;

    case ast::node_type::conditional_expression:
        print_json_class_head("conditional_expression");
        print_json_class_end();
        break;

    case ast::node_type::primary_expression:
        print_json_class_head("primary_expression");
        print_json_class_end();
        break;
    case ast::node_type::statement:
        print_json_class_head("statement");
        print_json_class_end();
        break;
    case ast::node_type::declare_varible:
        print_json_class_head("declare_varible");
        print_json_class_end();
        break;
    */
    case ast::node_type::initializer:
      print_json_class_head("initializer");
      dfs_print_tree(tree_body[idx].value.initializer.idx_assignment_expression);
      dfs_print_tree(tree_body[idx].value.initializer.idx_initializer_list);
      print_json_class_end();
      break;

    case ast::node_type::initializer_list:
      for (ast::idx i = idx; i != ast::null;
           i          = tree_body[i].value.initializer_list.idx_next_initializer_list)
      {
        // std::cout << " " << i << " "
        //           << tree_body[i].value.initializer_list.idx_next_initializer_list
        //           << std::endl;

        print_json_class_head("initializer_list");
        dfs_print_tree(tree_body[i].value.initializer_list.idx_head_initializer_list_node
        );
        dfs_print_tree(tree_body[i].value.initializer_list.idx_son_initializer_list);
        print_json_class_end();
      }
      // std::cout << "--------------" << std::endl;
      break;

    case ast::node_type::initializer_list_node:
      for (ast::idx i = idx; i != ast::null;
           i = tree_body[i].value.initializer_list_node.idx_next_initializer_list_node)
      {
        now_idx = i;
        print_json_class_head("initializer_list_node");
        print_json_constant(tree_body[i].value.initializer_list_node.idx_constant);
        print_json_class_end();
      }
      break;

    case ast::node_type::declare_function:
      print_json_class_head("declare_function");
      dfs_print_tree(tree_body[idx].value.declare_function.idx_function_arguments);
      dfs_print_tree(tree_body[idx].value.declare_function.idx_return_value_type);
      print_json_class_end();
      break;
    case ast::node_type::definition_function:
      print_json_class_head("definition_function");
      dfs_print_tree(tree_body[idx].value.definition_function.idx_function_arguments);
      dfs_print_tree(tree_body[idx].value.definition_function.idx_return_value_type);
      dfs_print_tree(tree_body[idx].value.definition_function.idx_compound_statement);
      print_json_class_end();
      break;
    case ast::node_type::initial_declarator_list:
      print_json_class_head("initial_declarator_list");
      dfs_print_tree(tree_body[idx].value.initial_declarator_list.idx_initial_declarator);
      print_json_class_end();
      break;

    case ast::node_type::definition_struct:
      print_json_class_head("definition_struct");
      print_json_class_end();
      break;

    case ast::node_type::constant:
      print_json_class_head("constant");
      print_json_key_value("value", tree_body[idx].value.constant.const_value);
      print_json_class_end();
      break;

    case ast::node_type::operators:
      print_json_class_head("operators");
      print_json_class_end();
      break;

    case ast::node_type::initial_declarator:
      print_json_class_head("initial_declarator");
      dfs_print_tree(tree_body[idx].value.initial_declarator.idx_declarator);
      dfs_print_tree(tree_body[idx].value.initial_declarator.idx_initializer);
      dfs_print_tree(tree_body[idx].value.initial_declarator.idx_next_initial_declarator);
      print_json_class_end();
      break;

    case ast::node_type::direct_declarator:
      print_json_class_head("direct_declarator");
      dfs_print_tree(tree_body[idx].value.direct_declarator.idx_identifier);
      dfs_print_tree(tree_body[idx].value.direct_declarator.idx_declarator);
      dfs_print_tree(tree_body[idx].value.direct_declarator.idx_array_declarator);
      if (!print_trans_tree)
      {
        dfs_print_tree(tree_body[idx].value.direct_declarator.idx_arguments_type_list);
      }
      print_json_class_end();
      break;

    case ast::node_type::array_declarator:
      print_json_class_head("array_declarator");
      dfs_print_tree(tree_body[idx].value.array_declarator.idx_constant);
      dfs_print_tree(tree_body[idx].value.array_declarator.idx_next_array_declarator);
      print_json_class_end();
      break;

    case ast::node_type::declarator:
      print_json_class_head("declarator");
      switch (tree_body[idx].value.declarator.is_ptr)
      {
        case 0:
          print_json_key_value("is_pointer", "0");
          break;
        case 1:
          print_json_key_value("is_pointer", "1");
          break;
        case 2:
          print_json_key_value("is_pointer", "2");
          break;
        case 3:
          print_json_key_value("is_pointer", "3");
          break;
        case 4:
          print_json_key_value("is_pointer", "4");
          break;
        case 5:
          print_json_key_value("is_pointer", "5");
          break;
        default:
          fmt::print("pointer level can't over 5");
          SWITCH_ERROR
      }
      dfs_print_tree(tree_body[idx].value.declarator.idx_direct_declarator);
      print_json_class_end();
      break;

    case ast::node_type::declaration_or_definition:
      print_json_class_head("declaration_or_definition");
      if (!print_trans_tree)
      {
        dfs_print_tree(
          tree_body[idx].value.declaration_or_definition.idx_declaration_declarator
        );
        dfs_print_tree(
          tree_body[idx].value.declaration_or_definition.idx_compound_statement
        );
      }
      dfs_print_tree(tree_body[idx].value.declaration_or_definition.idx_initial_declarator
      );
      dfs_print_tree(
        tree_body[idx].value.declaration_or_definition.idx_next_declaration_or_definition
      );
      print_json_class_end();
      break;

    case ast::node_type::identifier:
      print_json_class_head("identifier");
      print_json_key_value("name", tree_body[idx].value.identifier.name);
      print_json_class_end();
      break;

    case ast::node_type::arguments_type_list:
      print_json_class_head("arguments_type_list");
      dfs_print_tree(tree_body[idx].value.arguments_type_list.idx_argument_declaration);
      print_json_class_end();
      break;

    case ast::node_type::arguments_declaration:
      print_json_class_head("arguments_declaration");
      dfs_print_tree(tree_body[idx].value.arguments_declaration.idx_declaration_declarator
      );
      dfs_print_tree(tree_body[idx].value.arguments_declaration.idx_declarator);
      print_json_class_end();
      dfs_print_tree(
        tree_body[idx].value.arguments_declaration.idx_next_arguments_declaration
      );
      break;

    case ast::node_type::compound_statement:
      print_json_class_head("compound_statement");
      dfs_print_tree(tree_body[idx].value.compound_statement.idx_block);
      print_json_class_end();
      break;

    case ast::node_type::block:
      print_json_class_head("block");
      dfs_print_tree(tree_body[idx].value.block.idx_declaration);
      dfs_print_tree(tree_body[idx].value.block.idx_statement);
      print_json_class_end();
      dfs_print_tree(tree_body[idx].value.block.idx_next_block);
      break;

    case ast::node_type::postfix_operator:
      print_json_class_head("postfix_operator");
      switch (tree_body[idx].value.postfix_operator.postfix_operator)
      {
        case token::ver:
          print_json_key_value("postfix_operator", "->");
          return;
        case token::period:
          print_json_key_value("postfix_operator", ".");
          return;
        case token::self_plus:
          print_json_key_value("postfix_operator", "++");
          return;
        case token::self_minus:
          print_json_key_value("postfix_operator", "--");
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
      print_json_class_end();
      break;

    case ast::node_type::if_statement:
      print_json_class_head("if_statement");
      dfs_print_tree(tree_body[idx].value.if_statement.idx_assign_expression);
      dfs_print_tree(tree_body[idx].value.if_statement.idx_if_body);
      dfs_print_tree(tree_body[idx].value.if_statement.idx_else_body);
      print_json_class_end();
      break;

    case ast::node_type::for_statement:
      print_json_class_head("for_statement");
      dfs_print_tree(tree_body[idx].value.for_statement.idx_declaration);
      dfs_print_tree(tree_body[idx].value.for_statement.idx_conditional_assign_expression
      );
      dfs_print_tree(tree_body[idx].value.for_statement.idx_change_assign_expression);
      dfs_print_tree(tree_body[idx].value.for_statement.idx_compound_statement);
      print_json_class_end();
      break;

    case ast::node_type::switch_statement:
      print_json_class_head("switch_statement");
      dfs_print_tree(tree_body[idx].value.switch_statement.idx_assign_expression);
      dfs_print_tree(tree_body[idx].value.switch_statement.idx_compound_statement);
      print_json_class_end();
      break;

    case ast::node_type::do_while_statement:
      print_json_class_head("do_while_statement");
      dfs_print_tree(tree_body[idx].value.do_while_statement.idx_assign_statement);
      dfs_print_tree(tree_body[idx].value.do_while_statement.idx_compound_statement);
      print_json_class_end();
      break;

    case ast::node_type::continue_statement:
      print_json_class_head("continue_statement");
      print_json_class_end();
      break;

    case ast::node_type::break_statement:
      print_json_class_head("break_statement");
      print_json_class_end();
      break;

    case ast::node_type::default_label:
      print_json_class_head("default_label");
      print_json_class_end();
      break;

    case ast::node_type::case_label:
      print_json_class_head("case_label");
      dfs_print_tree(tree_body[idx].value.case_label.const_expression);
      print_json_class_end();
      break;

    // TODO
    case ast::node_type::basic_type_declaration:
      print_json_class_head("basic_type_declaration");
      dfs_print_tree(tree_body[idx].value.basic_type_declaration.idx_next);
      print_json_class_end();
      break;

    case ast::node_type::struct_declaration:
      print_json_class_head("struct_declaration");
      dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_type);
      dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_name);
      print_json_class_end();
      break;

    case ast::node_type::struct_definition:
      print_json_class_head("struct_definition");
      dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_type);
      dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_name);
      dfs_print_tree(tree_body[idx].value.struct_definition.idx_struct_body);
      print_json_class_end();
      break;

    case ast::node_type::declaration_declarator: {
      print_json_class_head("declaration_declarator");
      auto &decl = tree_body[idx].value.declaration_declarator;
      if (decl.sign == ast::declarator_sign::sign_signed)
      {
        print_json_key_value("sign", "true");
      }
      else
      {
        print_json_key_value("sign", "false");
      }

      print_json_key("type");
      switch (decl.type)
      {
        case ast::declarator_type::type_char:
          print_json_value("char");
          break;
        case ast::declarator_type::type_double:
          print_json_value("double");
          break;
        case ast::declarator_type::type_float:
          print_json_value("float");
          break;
        case ast::declarator_type::type_int:
          print_json_value("int");
          break;
        case ast::declarator_type::type_long_int:
          print_json_value("long_int");
          break;
        case ast::declarator_type::type_long_long_int:
          print_json_value("long_long_int");
          break;
        case ast::declarator_type::type_short_int:
          print_json_value("short_int");
          break;
        case ast::declarator_type::type_void:
          print_json_value("void");
          break;
        case ast::declarator_type::type_struct:
          print_json_value("struct");
          break;
        default:
          SWITCH_ERROR
      }

      print_json_key("limit");
      if (decl.limit == ast::declarator_limit::limit_Alignas)
      {
        print_json_value("Alignas");
      }
      else
      {
        print_json_value("empty");
      }

      print_json_key("store");
      switch (decl.store)
      {
        case ast::declarator_store::store_empty:
          print_json_value("empty");
          break;
        case ast::declarator_store::store_extern:
          print_json_value("extern");
          break;
        case ast::declarator_store::store_static:
          print_json_value("static");
          break;
        case ast::declarator_store::store_typedef:
          print_json_value("typedef");
          break;
        default:
          break;
      }
      dfs_print_tree(
        tree_body[idx].value.declaration_declarator.idx_struct_union_identifier
      );
      print_json_class_end();
      break;
    }


    // second step node
    case ast::node_type::function:
      print_json_class_head("function_declaration");
      dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_name);
      dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_declarator);
      dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_return_type);
      dfs_print_tree(
        tree_body[idx].value.function_declaration.idx_function_arguments_type_list
      );
      dfs_print_tree(tree_body[idx].value.function_declaration.idx_function_body);
      dfs_print_tree(tree_body[idx].value.function_declaration.idx_next);
      print_json_class_end();
      break;

    case ast::node_type::return_statement:
      print_json_class_head("return_statement");
      dfs_print_tree(tree_body[idx].value.return_statement.idx_assignment_expression);
      print_json_class_end();
      break;

    case ast::node_type::arguments_list_node:
      print_json_class_head("arguments_list_node");
      for (ast::idx i = idx; i != ast::null;
           i = tree_body[i].value.arguments_list_node.idx_next_arguments_list_node)
      {
        dfs_print_tree(tree_body[idx].value.arguments_list_node.idx_assignment_expression
        );
      }
      print_json_class_end();
      break;

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

void Tree::print_json_key_value(std::string key, std::string value)
{
  // file << "\"" << key << "\":\"" << value << "\",\n";

  file_buffer += fmt::format("\"{}\":\"{}\",", key, value);
  // file_buffer += "\"";
  // file_buffer += key;
  // file_buffer += "\":\"";
  // file_buffer += value;
  // file_buffer += "\",";
  return;
}

void Tree::print_json_class_head(std::string value)
{
  // file << "\"" <<  value << "\":{\n";

  // auto str_value = std::string(value);
  // file_buffer += fmt::format("\"{}\":{", str_value);
  file_buffer += "\"";
  file_buffer += value;
  file_buffer += '_';
  file_buffer += std::to_string(now_idx);
  file_buffer += "\":{";
  return;
}

void Tree::print_json_class_end()
{
  // file << "},\n";
  // file_buffer += fmt::format("},");
  file_buffer += "},";
  return;
}

void Tree::print_json_key(std::string key)
{
  // file << "\"" << key << "\":";
  // auto str_key = std::string(key);
  // file_buffer += fmt::format("\"{}\":", str_key);
  file_buffer += "\"";
  file_buffer += key;
  file_buffer += "\":";
  return;
}

void Tree::print_json_value(std::string value)
{
  //
  // file << "\"" << value << "\",\n";
  // auto str_value = std::string(value);
  // file_buffer += fmt::format("\"{}\",", str_value);
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