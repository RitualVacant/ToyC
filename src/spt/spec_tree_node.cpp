#ifndef SPEC_TREE_NODE_CPP
#define SPEC_TREE_NODE_CPP

#include "spec_tree_node.h"
#include "ast_node.h"
#include "global_var.h"
#include "inner.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <string>

namespace spt
{

uint64_t number_print_node = 0;

std::string get_node_name(std::string node_name)
{
  return node_name + std::to_string(number_print_node);
}


////////////////////////////////////////////////////////////////
/// @brief Base
////////////////////////////////////////////////////////////////

void spt::Base::print()
{
  NOT_REACHABLE
}

////////////////////////////////////////////////////////////////
/// @brief Statement
////////////////////////////////////////////////////////////////

void spt::Statement::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief Block
////////////////////////////////////////////////////////////////

spt::Block::Block()
{
}
spt::Block::~Block() {}
spt::Block *spt::Block::create()
{
  // spt::ptr_tree_body->push_back(spt::Block());
  // return &(std::get<spt::Block>(spt::ptr_tree_body->back()));
  return new spt::Block();
}

void spt::Block::push_back(Statement *ptr_statement)
{
  block_body.push_back(ptr_statement);
}

void spt::Block::push_back(Block *block)
{
  block_body.insert(block_body.end(), block->block_body.begin(), block->block_body.end());
}

uint64_t spt::Block::size() const
{
  return block_body.size();
}

void spt::Block::print()
{
  json.print_json_class_head("Block");
  for (auto i : block_body)
  {
    i->print();
  }
  json.print_json_class_end();
}

////////////////////////////////////////////////////////////////
/// @brief Def
////////////////////////////////////////////////////////////////

void spt::Def::print()
{
  NOT_REACHABLE
}

////////////////////////////////////////////////////////////////
/// @brief Dec
////////////////////////////////////////////////////////////////

void spt::Dec::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief Type
////////////////////////////////////////////////////////////////
spt::Type basic_type_loc[20];

spt::Type::Type() {}

spt::Type::~Type() {}

spt::Type *spt::Type::get_int8()
{
  return &basic_type_loc[0];
}

spt::Type *spt::Type::get_int16()
{
  return &basic_type_loc[1];
}

spt::Type *spt::Type::get_int32()
{
  return &basic_type_loc[2];
}

spt::Type *spt::Type::get_int64()
{
  return &basic_type_loc[3];
}

spt::Type *spt::Type::get_uint8()
{
  return &basic_type_loc[4];
}

spt::Type *spt::Type::get_uint16()
{
  return &basic_type_loc[5];
}

spt::Type *spt::Type::get_uint32()
{
  return &basic_type_loc[6];
}

spt::Type *spt::Type::get_uint64()
{
  return &basic_type_loc[7];
}

spt::Type *spt::Type::get_void()
{
  return &basic_type_loc[8];
}

spt::Type *spt::Type::get_struct()
{
  return &basic_type_loc[9];
}

spt::Type *spt::Type::get_double()
{
  return &basic_type_loc[10];
}

spt::Type *spt::Type::get_float()
{
  return &basic_type_loc[11];
}

void spt::Type::print()
{
  if (this == basic_type_loc + 0)
  {
    json.print_json_key_value("type", "int8");
    return;
  }
  if (this == basic_type_loc + 1)
  {
    json.print_json_key_value("type", "int16");
    return;
  }
  if (this == basic_type_loc + 2)
  {
    json.print_json_key_value("type", "int32");
    return;
  }
  if (this == basic_type_loc + 3)
  {
    json.print_json_key_value("type", "int64");
    return;
  }
  if (this == basic_type_loc + 4)
  {
    json.print_json_key_value("type", "uint8");
    return;
  }
  if (this == basic_type_loc + 5)
  {
    json.print_json_key_value("type", "uint16");
    return;
  }
  if (this == basic_type_loc + 6)
  {
    json.print_json_key_value("type", "uint32");
    return;
  }
  if (this == basic_type_loc + 7)
  {
    json.print_json_key_value("type", "uint64");
    return;
  }
  if (this == basic_type_loc + 8)
  {
    json.print_json_key_value("type", "void");
    return;
  }
  if (this == basic_type_loc + 9)
  {
    json.print_json_key_value("type", "struct");
    return;
  }
  if (this == basic_type_loc + 10)
  {
    json.print_json_key_value("type", "double");
    return;
  }
  if (this == basic_type_loc + 11)
  {
    json.print_json_key_value("type", "float");
    return;
  }
}

////////////////////////////////////////////////////////////////
/// @brief FunctionType
////////////////////////////////////////////////////////////////
spt::FuncType::FuncType(
  spt::Type               *ptr_return_type_,
  std::vector<spt::Type *> argument_type_list_
)
    : ptr_return_type{ptr_return_type_}, argument_type_list{argument_type_list_}
{
}

spt::FuncType::~FuncType() {}

spt::FuncType *
spt::FuncType::get(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list)
{
  // spt::ptr_tree_body->push_back(spt::FuncType(ptr_return_type, argument_type_list));
  // return &std::get<FuncType>(ptr_tree_body->back());
  return new spt::FuncType(ptr_return_type, argument_type_list);
}

void spt::FuncType::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief ArrayType
////////////////////////////////////////////////////////////////

spt::ArrayType *spt::ArrayType::get(
  Type                      *ptr_unit_type,
  std::vector<std::uint64_t> dimension_len
){TODO};

void spt::ArrayType::print(){TODO};

////////////////////////////////////////////////////////////////
/// @brief PointerType
////////////////////////////////////////////////////////////////

spt::PointerType *spt::PointerType::get(Type *ptr_type_)
{
  // spt::ptr_tree_body->push_back(spt::PointerType(ptr_type_));
  // return &std::get<spt::PointerType>(spt::ptr_tree_body->back());
  return new spt::PointerType(ptr_type_);
}

spt::PointerType::PointerType(Type *ptr_type_) : ptr_element_type{ptr_type_} {}
spt::PointerType::~PointerType() {}
void spt::PointerType::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief Func
////////////////////////////////////////////////////////////////

spt::Func::Func(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_

)
    : ptr_return_type{ptr_return_type_}, name{name_}, argument_type_list{
                                                        argument_type_list_} {};
spt::Func::~Func(){};
void spt::Func::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief FuncDef
////////////////////////////////////////////////////////////////


spt::FuncDef::FuncDef(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
  Block                                                         *body_
)
    : Func(ptr_return_type_, name_, argument_type_list_), body{body_} {};

spt::FuncDef *spt::FuncDef::create(
  spt::Type                                                     *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
  spt::Block                                                    *body_
)
{
  // spt::ptr_tree_body->push_back(
  // spt::FuncDef(ptr_return_type_, name_, argument_type_list_, body_)
  // );
  // return &std::get<spt::FuncDef>(ptr_tree_body->back());
  return new spt::FuncDef(ptr_return_type_, name_, argument_type_list_, body_);
}

void spt::FuncDef::print()
{
  json.print_json_class_head("FuncDef");
  json.print_json_key_value("name", name);
  json.print_json_class_head("argument_type_list");
  for (auto i : std::get<0>(argument_type_list))
  {
    i->print();
  }
  json.print_json_class_end();
  body->print();
  json.print_json_class_end();
}

////////////////////////////////////////////////////////////////
/// @brief FuncDec
////////////////////////////////////////////////////////////////

spt::FuncDec::FuncDec(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_
)
    : Func(ptr_return_type_, name_, argument_type_list_)
{
}

spt::FuncDec *spt::FuncDec::create(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_
)
{
  // ptr_tree_body->push_back(FuncDec(ptr_return_type_, name_, argument_type_list_));
  // return &std::get<FuncDec>(ptr_tree_body->back());
  return new FuncDec(ptr_return_type_, name_, argument_type_list_);
}

void spt::FuncDec::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief VarDef
////////////////////////////////////////////////////////////////

spt::VarDef::VarDef(spt::Type *type_, std::string name_, spt::Expr *initializer_)
    : Var{name_, type_}, initializer{initializer_}
{
}

spt::VarDef *
spt::VarDef::create(spt::Type *type_, std::string name_, spt::Expr *initializer_)
{
  // ptr_tree_body->push_back(VarDef(type_, name_, initializer_));
  // return &std::get<VarDef>(ptr_tree_body->back());
  return new VarDef(type_, name_, initializer_);
}

void spt::VarDef::print()
{
  json.print_json_class_head("VarDef");
  json.print_json_key_value("identifier:", identifier);
  type->print();
  json.print_json_class_end();
}

////////////////////////////////////////////////////////////////
/// @brief StrcutDef
////////////////////////////////////////////////////////////////

spt::StructDef *spt::StructDef::create(
  std::string                                                    name,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> element_list
)
{
  TODO
}

void spt::StructDef::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief StructDec
////////////////////////////////////////////////////////////////

spt::StructDec *spt::StructDec::create(std::string name){TODO};
void spt::StructDec::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief IfStatement
////////////////////////////////////////////////////////////////

spt::IfStatement::IfStatement(
  spt::Expr  *expr,
  spt::Block *true_block,
  spt::Block *false_block
)
    : expr{expr}, true_block{true_block}, false_block{false_block}
{
}

spt::IfStatement *
spt::IfStatement::create(spt::Expr *expr, spt::Block *true_block, spt::Block *false_block)
{
  // ptr_tree_body->push_back(IfStatement(expr, true_block, false_block));
  // return &std::get<IfStatement>(ptr_tree_body->back());
  return new IfStatement(expr, true_block, false_block);
}

spt::IfStatement *spt::IfStatement::create(spt::Expr *expr, spt::Block *true_block)
{
  // ptr_tree_body->push_back(IfStatement(expr, true_block, nullptr));
  // return &std::get<IfStatement>(ptr_tree_body->back());
  return new IfStatement(expr, true_block, nullptr);
}

void spt::IfStatement::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief ReturnStatement
////////////////////////////////////////////////////////////////

spt::ReturnStatement::ReturnStatement(Expr *return_expr_)
    : return_expr{return_expr_}
{
}
spt::ReturnStatement::~ReturnStatement() {}
spt::ReturnStatement *spt::ReturnStatement::create(Expr *return_expr)
{
  // ptr_tree_body->push_back(ReturnStatement(return_expr));
  // return &std::get<ReturnStatement>(ptr_tree_body->back());
  return new ReturnStatement(return_expr);
}
void spt::ReturnStatement::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief WhileStatement
////////////////////////////////////////////////////////////////

spt::WhileStatement::WhileStatement(spt::Expr *expr_, spt::Block *while_body_)
    : expr{expr_}, while_body{while_body_}
{
}
spt::WhileStatement::~WhileStatement() {}
spt::WhileStatement *spt::WhileStatement::create(spt::Expr *expr, spt::Block *while_body)
{
  // ptr_tree_body->push_back(WhileStatement(expr, while_body));
  // return &std::get<WhileStatement>(ptr_tree_body->back());
  return new WhileStatement(expr, while_body);
}

void spt::WhileStatement::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief DoWhileStatement
////////////////////////////////////////////////////////////////

spt::DoWhileStatement::DoWhileStatement(spt::Expr *expr_, spt::Block *do_while_body_)
    : expr{expr_}, do_while_body{do_while_body_}
{
}
spt::DoWhileStatement::~DoWhileStatement() {}
spt::DoWhileStatement *
spt::DoWhileStatement::create(spt::Expr *expr, spt::Block *do_while_body)
{
  // ptr_tree_body->push_back(DoWhileStatement(expr, do_while_body));
  // return &std::get<DoWhileStatement>(ptr_tree_body->back());
  return new DoWhileStatement(expr, do_while_body);
}

void spt::DoWhileStatement::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief ForStatement
////////////////////////////////////////////////////////////////

spt::ForStatement::ForStatement(
  spt::Expr  *init_expr_,
  spt::Expr  *condition_expr_,
  spt::Expr  *change_expr_,
  spt::Block *for_body_
)
    : init_expr(init_expr_), condition_expr{condition_expr_},
      change_expr{change_expr_}, for_body{for_body_}
{
}
spt::ForStatement::ForStatement(
  spt::Def   *init_def_,
  spt::Expr  *condition_expr_,
  spt::Expr  *change_expr_,
  spt::Block *for_body_
)
    : init_def{init_def_}, condition_expr{condition_expr_},
      change_expr{change_expr_}, for_body{for_body_}
{
}

spt::ForStatement::~ForStatement() {}

spt::ForStatement *spt::ForStatement::create(
  spt::Expr  *init_expr,
  spt::Expr  *condition_expr,
  spt::Expr  *change_expr,
  spt::Block *for_body
){TODO}

spt::ForStatement *spt::ForStatement::create(
  spt::Def   *init_def,
  spt::Expr  *condition_expr,
  spt::Expr  *change_expr,
  spt::Block *for_body
)
{
  TODO
}

void spt::ForStatement::print()
{
  TODO
}

////////////////////////////////////////////////////////////////
/// @brief ForStatement
////////////////////////////////////////////////////////////////

void spt::SwitchStatement::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief Expr
////////////////////////////////////////////////////////////////
spt::Expr::Expr(token op_, Expr *l_expr_, Expr *r_expr_)
    : op{op_}, l_expr{l_expr_}, r_expr{r_expr_}
{
}

spt::Expr::Expr() {}

spt::Expr::~Expr() {}

spt::Expr *spt::Expr::create(token op, Expr *l_expr, Expr *r_expr)
{
  // ptr_tree_body->push_back(Expr(op, l_expr, r_expr));
  // return &std::get<Expr>(ptr_tree_body->back());
  return new Expr(op, l_expr, r_expr);
}

void spt::Expr::print(){TODO}
////////////////////////////////////////////////////////////////
/// @brief Array
////////////////////////////////////////////////////////////////

spt::Array::Array(std::string identifier_, std::vector<Expr *> idx_list_)
    : identifier{identifier_}, idx_list{idx_list_}
{
}

spt::Array::~Array() {}

spt::Array *spt::Array::create(std::string identifier, std::vector<Expr *> idx_list)
{
  // ptr_tree_body->push_back(Array(identifier, idx_list));
  // return &std::get<Array>(ptr_tree_body->back());
  return new Array(identifier, idx_list);
}

void spt::Array::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief FuncCall
////////////////////////////////////////////////////////////////
spt::FuncCall::FuncCall(std::string identifier_, std::vector<spt::Expr *> argument_list_)
    : identifier{identifier_}, argument_list{argument_list_}
{
}

spt::FuncCall::~FuncCall() {}

spt::FuncCall *
spt::FuncCall::create(std::string identifier, std::vector<Expr *> argument_list)
{
  // ptr_tree_body->push_back(FuncCall(identifier, argument_list));
  // return &std::get<FuncCall>(ptr_tree_body->back());
  return new FuncCall(identifier, argument_list);
}

void spt::FuncCall::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief Var
////////////////////////////////////////////////////////////////

spt::Var::Var(std::string identifier_, spt::Type *type_)
    : identifier{identifier_}, type{type_}
{
}

spt::Var::~Var() {}

spt::Var *spt::Var::create(std::string identifier, spt::Type *type)
{
  // ptr_tree_body->push_back(Var(identifier));
  // return &std::get<Var>(ptr_tree_body->back());
  return new Var(identifier, type);
}

void spt::Var::print()
{
  NOT_REACHABLE
}

////////////////////////////////////////////////////////////////
/// @brief VarDec
////////////////////////////////////////////////////////////////

void spt::VarDec::print()
{
  TODO
}

}  // namespace spt

#endif