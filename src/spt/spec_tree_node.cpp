#ifndef SPEC_TREE_NODE_CPP
#define SPEC_TREE_NODE_CPP

#include "spec_tree_node.h"
#include "ast_node.h"
#include "global_var.h"
#include "inner.h"
#include "token.h"
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

void Base::print()
{
  NOT_REACHABLE
}

////////////////////////////////////////////////////////////////
/// @brief Statement
////////////////////////////////////////////////////////////////

void Statement::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief Block
////////////////////////////////////////////////////////////////

Block::Block()
{
}
Block::~Block() {}
Block *Block::create()
{
  // ptr_tree_body->push_back(Block());
  // return &(std::get<Block>(ptr_tree_body->back()));
  return new Block();
}

void Block::push_back(Statement *ptr_statement)
{
  block_body.push_back(ptr_statement);
}

void Block::push_back(Block *block)
{
  block_body.insert(block_body.end(), block->block_body.begin(), block->block_body.end());
}

uint64_t Block::size() const
{
  return block_body.size();
}

void Block::print()
{
  json.print_class("Block", [&] {
    for (size_t i = 0; i < block_body.size(); ++i)
    {
      json.print_class("statement-" + std::to_string(i), [&] { block_body[i]->print(); });
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief Def
////////////////////////////////////////////////////////////////

void Def::print()
{
  NOT_REACHABLE
}

////////////////////////////////////////////////////////////////
/// @brief Dec
////////////////////////////////////////////////////////////////

void Dec::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief Type
////////////////////////////////////////////////////////////////
Type basic_type_loc[20];

Type::Type() {}

Type::~Type() {}

Type *Type::get_int8()
{
  return &basic_type_loc[0];
}

Type *Type::get_int16()
{
  return &basic_type_loc[1];
}

Type *Type::get_int32()
{
  return &basic_type_loc[2];
}

Type *Type::get_int64()
{
  return &basic_type_loc[3];
}

Type *Type::get_uint8()
{
  return &basic_type_loc[4];
}

Type *Type::get_uint16()
{
  return &basic_type_loc[5];
}

Type *Type::get_uint32()
{
  return &basic_type_loc[6];
}

Type *Type::get_uint64()
{
  return &basic_type_loc[7];
}

Type *Type::get_void()
{
  return &basic_type_loc[8];
}

Type *Type::get_struct()
{
  return &basic_type_loc[9];
}

Type *Type::get_double()
{
  return &basic_type_loc[10];
}

Type *Type::get_float()
{
  return &basic_type_loc[11];
}

Type *Type::get_udouble()
{
  return &basic_type_loc[11];
}

Type *Type::get_ufloat()
{
  return &basic_type_loc[12];
}

void Type::print()
{
  if (this == basic_type_loc + 0)
  {
    json.print_key_value("type", "int8");
    return;
  }
  if (this == basic_type_loc + 1)
  {
    json.print_key_value("type", "int16");
    return;
  }
  if (this == basic_type_loc + 2)
  {
    json.print_key_value("type", "int32");
    return;
  }
  if (this == basic_type_loc + 3)
  {
    json.print_key_value("type", "int64");
    return;
  }
  if (this == basic_type_loc + 4)
  {
    json.print_key_value("type", "uint8");
    return;
  }
  if (this == basic_type_loc + 5)
  {
    json.print_key_value("type", "uint16");
    return;
  }
  if (this == basic_type_loc + 6)
  {
    json.print_key_value("type", "uint32");
    return;
  }
  if (this == basic_type_loc + 7)
  {
    json.print_key_value("type", "uint64");
    return;
  }
  if (this == basic_type_loc + 8)
  {
    json.print_key_value("type", "void");
    return;
  }
  if (this == basic_type_loc + 9)
  {
    json.print_key_value("type", "struct");
    return;
  }
  if (this == basic_type_loc + 10)
  {
    json.print_key_value("type", "double");
    return;
  }
  if (this == basic_type_loc + 11)
  {
    json.print_key_value("type", "float");
    return;
  }
  if (this == basic_type_loc + 12)
  {
    json.print_key_value("type", "udouble");
    return;
  }
  if (this == basic_type_loc + 13)
  {
    json.print_key_value("type", "ufloat");
    return;
  }
}

////////////////////////////////////////////////////////////////
/// @brief FunctionType
////////////////////////////////////////////////////////////////
FuncType::FuncType(Type *ptr_return_type_, std::vector<Type *> argument_type_list_)
    : ptr_return_type{ptr_return_type_}, argument_type_list{argument_type_list_}
{
}

FuncType::~FuncType() {}

FuncType *FuncType::get(Type *ptr_return_type, std::vector<Type *> argument_type_list)
{
  // ptr_tree_body->push_back(FuncType(ptr_return_type, argument_type_list));
  // return &std::get<FuncType>(ptr_tree_body->back());
  return new FuncType(ptr_return_type, argument_type_list);
}

void FuncType::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief ArrayType
////////////////////////////////////////////////////////////////
ArrayType::ArrayType(Type *ptr_unit_type_, std::vector<Expr *> dimension_len_expr_)
    : ptr_unit_type{ptr_unit_type_}, dimension_len_expr{dimension_len_expr_}
{
}
ArrayType *ArrayType::get(Type *ptr_unit_type, std::vector<Expr *> dimension_len_expr)
{
  return new ArrayType(ptr_unit_type, dimension_len_expr);
};

void ArrayType::print()
{
  json.print_class("ArrayType", [&] {
    json.print_class("unit_type", [&] { ptr_unit_type->print(); });
    json.print_array("dimension_len", [&] {
      for (auto i : dimension_len_expr)
      {
        json.print_array_item([&] { i->print(); });
      }
    });
  });
};

////////////////////////////////////////////////////////////////
/// @brief PointerType
////////////////////////////////////////////////////////////////

PointerType *PointerType::get(Type *ptr_type_)
{
  return new PointerType(ptr_type_);
}

PointerType::PointerType(Type *ptr_type_) : ptr_element_type{ptr_type_} {}
PointerType::~PointerType() {}
void PointerType::print()
{
  json.print_class("PointerType", [&] {
    json.print_class("element_type", [&] { ptr_element_type->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief Func
////////////////////////////////////////////////////////////////

Func::Func(
  Type                                                     *ptr_return_type_,
  std::string                                               name_,
  std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list_

)
    : ptr_return_type{ptr_return_type_}, name{name_}, argument_type_list{
                                                        argument_type_list_} {};
Func::~Func(){};
void Func::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief FuncDef
////////////////////////////////////////////////////////////////


FuncDef::FuncDef(
  Type                                                     *ptr_return_type_,
  std::string                                               name_,
  std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list_,
  Block                                                    *body_
)
    : Func(ptr_return_type_, name_, argument_type_list_), body{body_} {};

FuncDef *FuncDef::create(
  Type                                                     *ptr_return_type_,
  std::string                                               name_,
  std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list_,
  Block                                                    *body_
)
{
  return new FuncDef(ptr_return_type_, name_, argument_type_list_, body_);
}

void FuncDef::print()
{
  json.print_class("FuncDef", [&] {
    json.print_key_value("name", name);
    json.print_array("argument_type_list", [&] {
      for (auto i : std::get<0>(argument_type_list))
      {
        i->print();
      }
    });
    json.print_class("body", [&] { body->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief FuncDec
////////////////////////////////////////////////////////////////

FuncDec::FuncDec(
  Type                                                     *ptr_return_type_,
  std::string                                               name_,
  std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list_
)
    : Func(ptr_return_type_, name_, argument_type_list_)
{
}

FuncDec *FuncDec::create(
  Type                                                     *ptr_return_type_,
  std::string                                               name_,
  std::tuple<std::vector<Type *>, std::vector<std::string>> argument_type_list_
)
{
  return new FuncDec(ptr_return_type_, name_, argument_type_list_);
}

void FuncDec::print()
{
  json.print_class("FuncDec", [&] {
    json.print_key_value("name", name);
    json.print_array("argument_type_list", [&] {
      for (auto i : std::get<0>(argument_type_list))
      {
        i->print();
      }
    });
  });
}

////////////////////////////////////////////////////////////////
/// @brief Constant
////////////////////////////////////////////////////////////////
Constant::Constant(std::string value_) : value{value_} {}

void Constant::print(){NOT_REACHABLE}

////////////////////////////////////////////////////////////////
/// @brief ConstantInt
////////////////////////////////////////////////////////////////

ConstantInt::ConstantInt(std::string value_, bool negative_)
    : Constant{value_}, negative{negative_}
{
}

ConstantInt *ConstantInt::create(std::string value, bool negative)
{
  return new ConstantInt(value, negative);
}

void ConstantInt::print()
{
  json.print_class("ConstantInt", [&] {
    json.print_key_value("value", value);
    if (negative)
    {
      json.print_key_value("negative", "true");
    }
    else
    {
      json.print_key_value("negative", "false");
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief ConstantFloat
////////////////////////////////////////////////////////////////

ConstantFloat::ConstantFloat(std::string value_, bool negative_)
    : Constant{value_}, negative{negative_}
{
}

ConstantFloat *ConstantFloat::create(std::string value_, bool negative_)
{
  return new ConstantFloat(value_, negative_);
}

void ConstantFloat::print()
{
  json.print_class("ConstantFloat", [&] {
    json.print_key_value("value", value);
    if (negative)
    {
      json.print_key_value("negative", "true");
    }
    else
    {
      json.print_key_value("negative", "false");
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief StringLiteral
////////////////////////////////////////////////////////////////

StringLiteral::StringLiteral(std::string value_) : Constant{value_} {}

StringLiteral *StringLiteral::create(std::string value)
{
  return new StringLiteral(value);
}

void StringLiteral::print()
{
  json.print_class("StringLiteral", [&] { json.print_key_value("value", value); });
}

////////////////////////////////////////////////////////////////
/// @brief VarDef
////////////////////////////////////////////////////////////////

VarDef::VarDef(Type *type_, std::string identifier_, Expr *initializer_)
    : identifier{identifier_}, type{type_}, initializer{initializer_}
{
}

VarDef *VarDef::create(Type *type_, std::string name_, Expr *initializer_)
{
  // ptr_tree_body->push_back(VarDef(type_, name_, initializer_));
  // return &std::get<VarDef>(ptr_tree_body->back());
  return new VarDef(type_, name_, initializer_);
}

void VarDef::print()
{
  json.print_class("VarDef", [&] {
    json.print_key_value("identifier:", identifier);
    type->print();
    if (initializer != nullptr)
    {
      json.print_class("initializer", [&] { initializer->print(); });
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief Conversion
////////////////////////////////////////////////////////////////

Conversion::Conversion(Type *type_, Expr *expr_) : type{type_}, expr{expr_} {};

Conversion *Conversion::create(Type *type, Expr *expr)
{
  return new Conversion(type, expr);
}

void Conversion::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief Conversion
////////////////////////////////////////////////////////////////

UnaryExpr::UnaryExpr(token unary_op_, Expr *expr_)
    : unary_op{unary_op_}, expr{expr_}
{
}

UnaryExpr *UnaryExpr::create(token unary_op, Expr *expr)
{
  return new UnaryExpr(unary_op, expr);
}

void UnaryExpr::print()
{
  json.print_class("UnaryExpr", [&] {
    json.print_key_value("unary operator", operator_token_to_symbol(unary_op));
    json.print_class("expr", [&] { expr->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief StrcutDef
////////////////////////////////////////////////////////////////

StructDef *StructDef::create(
  std::string                                               name,
  std::tuple<std::vector<Type *>, std::vector<std::string>> element_list
)
{
  TODO
}

void StructDef::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief StructDec
////////////////////////////////////////////////////////////////

StructDec *StructDec::create(std::string name){TODO};
void StructDec::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief IfStatement
////////////////////////////////////////////////////////////////

IfStatement::IfStatement(Expr *expr, Block *true_block, Block *false_block)
    : expr{expr}, true_block{true_block}, false_block{false_block}
{
}

IfStatement *IfStatement::create(Expr *expr, Block *true_block, Block *false_block)
{
  // ptr_tree_body->push_back(IfStatement(expr, true_block, false_block));
  // return &std::get<IfStatement>(ptr_tree_body->back());
  return new IfStatement(expr, true_block, false_block);
}

IfStatement *IfStatement::create(Expr *expr, Block *true_block)
{
  // ptr_tree_body->push_back(IfStatement(expr, true_block, nullptr));
  // return &std::get<IfStatement>(ptr_tree_body->back());
  return new IfStatement(expr, true_block, nullptr);
}

void IfStatement::print()
{
  json.print_class("IfStatement", [&] {
    json.print_class("expr", [&] { expr->print(); });
    json.print_class("true_block", [&] { true_block->print(); });
    json.print_class("false_block", [&] {
      if (false_block != nullptr)
      {
        false_block->print();
      }
    });
  });
}

////////////////////////////////////////////////////////////////
/// @brief ReturnStatement
////////////////////////////////////////////////////////////////

ReturnStatement::ReturnStatement(Expr *return_expr_) : return_expr{return_expr_} {}
ReturnStatement::~ReturnStatement() {}
ReturnStatement *ReturnStatement::create(Expr *return_expr)
{
  // ptr_tree_body->push_back(ReturnStatement(return_expr));
  // return &std::get<ReturnStatement>(ptr_tree_body->back());
  return new ReturnStatement(return_expr);
}
void ReturnStatement::print()
{
  json.print_class("ReturnStatement", [&] {
    if (return_expr != nullptr)
    {
      return_expr->print();
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief WhileStatement
////////////////////////////////////////////////////////////////

WhileStatement::WhileStatement(Expr *expr_, Block *while_body_)
    : expr{expr_}, while_body{while_body_}
{
}
WhileStatement::~WhileStatement() {}
WhileStatement *WhileStatement::create(Expr *expr, Block *while_body)
{
  // ptr_tree_body->push_back(WhileStatement(expr, while_body));
  // return &std::get<WhileStatement>(ptr_tree_body->back());
  return new WhileStatement(expr, while_body);
}

void WhileStatement::print()
{
  json.print_class("WhileStatement", [&] {
    json.print_class("expr", [&] { expr->print(); });
    json.print_class("while_body", [&] { while_body->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief DoWhileStatement
////////////////////////////////////////////////////////////////

DoWhileStatement::DoWhileStatement(Expr *expr_, Block *do_while_body_)
    : expr{expr_}, do_while_body{do_while_body_}
{
}
DoWhileStatement::~DoWhileStatement() {}
DoWhileStatement *DoWhileStatement::create(Expr *expr, Block *do_while_body)
{
  // ptr_tree_body->push_back(DoWhileStatement(expr, do_while_body));
  // return &std::get<DoWhileStatement>(ptr_tree_body->back());
  return new DoWhileStatement(expr, do_while_body);
}

void DoWhileStatement::print()
{
  json.print_class("DoWhileStatement", [&] {
    json.print_class("expr", [&] { expr->print(); });
    json.print_class("do_while_body", [&] { do_while_body->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief ForStatement
////////////////////////////////////////////////////////////////

ForStatement::ForStatement(
  Block *init_block_,
  Expr  *condition_expr_,
  Expr  *change_expr_,
  Block *for_body_
)
    : init_block(init_block_), condition_expr{condition_expr_},
      change_expr{change_expr_}, for_body{for_body_}
{
}

ForStatement::~ForStatement() {}

ForStatement *ForStatement::create(
  Block *init_expr,
  Expr  *condition_expr,
  Expr  *change_expr,
  Block *for_body
)
{
  return new ForStatement(init_expr, condition_expr, change_expr, for_body);
}

void ForStatement::print()
{
  json.print_class("ForStatement", [&] {
    json.print_class("init_block", [&] { init_block->print(); });
    json.print_class("condition_expr", [&] { condition_expr->print(); });
    json.print_class("change_expr", [&] { change_expr->print(); });
    json.print_class("for_body", [&] { for_body->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief ForStatement
////////////////////////////////////////////////////////////////

void SwitchStatement::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief Expr
////////////////////////////////////////////////////////////////
Expr::Expr(token op_, Expr *l_expr_, Expr *r_expr_)
    : op{op_}, l_expr{l_expr_}, r_expr{r_expr_}
{
}

Expr::Expr() {}

Expr::~Expr() {}

Expr *Expr::create(token op, Expr *l_expr, Expr *r_expr)
{
  // ptr_tree_body->push_back(Expr(op, l_expr, r_expr));
  // return &std::get<Expr>(ptr_tree_body->back());
  return new Expr(op, l_expr, r_expr);
}

void Expr::print()
{
  json.print_class("Expr", [&] {
    json.print_key_value("operator", operator_token_to_symbol(op));
    if (l_expr != nullptr)
    {
      json.print_class("l_expr", [&] { l_expr->print(); });
    }
    if (r_expr != nullptr)
    {
      json.print_class("r_expr", [&] { r_expr->print(); });
    }
  });
}
////////////////////////////////////////////////////////////////
/// @brief Array
////////////////////////////////////////////////////////////////

Array::Array(std::string identifier_, std::vector<Expr *> idx_list_)
    : identifier{identifier_}, idx_list{idx_list_}
{
}

Array::~Array() {}

Array *Array::create(std::string identifier, std::vector<Expr *> idx_list)
{
  return new Array(identifier, idx_list);
}

void Array::print()
{
  json.print_class("Array", [&] {
    json.print_key_value("identifier", identifier);
    json.print_array("idx_list", [&] {
      for (auto i : idx_list)
      {
        json.print_array_item([&] { i->print(); });
      }
    });
  });
}


////////////////////////////////////////////////////////////////
/// @brief ArrayDecl
////////////////////////////////////////////////////////////////

ArrayDecl::ArrayDecl(
  Type        *array_type_,
  std::string  identifier_,
  Initializer *initializer_
)
    : array_type{array_type_}, identifier{identifier_}, initializer{initializer_}
{
}

ArrayDecl *
ArrayDecl::create(Type *array_type, std::string identifier, Initializer *initializer)
{
  return new ArrayDecl(array_type, identifier, initializer);
}
void ArrayDecl::print()
{
  json.print_class("ArrayDecl", [&] {
    json.print_class("array_type", [&] { array_type->print(); });
    json.print_key_value("identifier", identifier);
    json.print_class("initializer", [&] { initializer->print(); });
  });
}

////////////////////////////////////////////////////////////////
/// @brief ArrayDef
////////////////////////////////////////////////////////////////

ArrayDef::ArrayDef(Type *array_type_, std::string identifier_, Initializer *initializer_)
    : array_type{array_type_}, identifier{identifier_}, initializer{initializer_}
{
}

ArrayDef *
ArrayDef::create(Type *array_type, std::string identifier, Initializer *initializer)
{
  return new ArrayDef(array_type, identifier, initializer);
}

void ArrayDef::print()
{
  json.print_class("ArrayDecl", [&] {
    json.print_class("array_type", [&] { array_type->print(); });
    json.print_key_value("identifier", identifier);
    if (initializer != nullptr)
    {
      json.print_class("initializer", [&] { initializer->print(); });
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief FuncCall
////////////////////////////////////////////////////////////////
FuncCall::FuncCall(std::string identifier_, std::vector<Expr *> argument_list_)
    : identifier{identifier_}, argument_list{argument_list_}
{
}

FuncCall::~FuncCall() {}

FuncCall *FuncCall::create(std::string identifier, std::vector<Expr *> argument_list)
{
  // ptr_tree_body->push_back(FuncCall(identifier, argument_list));
  // return &std::get<FuncCall>(ptr_tree_body->back());
  return new FuncCall(identifier, argument_list);
}

void FuncCall::print()
{
  json.print_class(" FuncOperator", [&] {
    for (auto i : argument_list)
    {
      json.print_array_item([&] { i->print(); });
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief Var
////////////////////////////////////////////////////////////////

Var::Var(std::string identifier_, Type *type_, PostfixOperator *next_postfix_operator_)
    : identifier{identifier_}, type{type_}, next_postfix_operator{next_postfix_operator_}
{
}

Var::~Var() {}

Var *Var::create(
  std::string      identifier,
  Type            *type,
  PostfixOperator *next_postfix_operator
)
{
  return new Var(identifier, type, next_postfix_operator);
}

void Var::print()
{
  json.print_class("Var", [&] {
    json.print_key_value("identifier", identifier);
    json.print_class("type", [&] { type->print(); });
    if (next_postfix_operator != nullptr)
    {
      json.print_class("next_postfix_operator", [&] { next_postfix_operator->print(); });
    }
  });
}

////////////////////////////////////////////////////////////////
/// @brief VarDec
////////////////////////////////////////////////////////////////

void VarDec::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief ArrayOperator
////////////////////////////////////////////////////////////////

ArrayOperator::ArrayOperator(
  std::vector<Expr *> &idx_list_,
  PostfixOperator     *next_postfix_operator_
)
    : idx_list{idx_list_}, PostfixOperator{next_postfix_operator_}
{
}

void ArrayOperator::print()
{
  json.print_class(" ArrayOperator", [&] {
    json.print_array("arguments_list", [&] {
      for (auto i : idx_list)
      {
        json.print_array_item([&] { i->print(); });
      }
    });
    json.print_class("next_postfix_operator", [&] { next_postfix_operator->print(); });
  });
}

ArrayOperator *ArrayOperator::create(
  std::vector<Expr *> &idx_list,
  PostfixOperator     *next_postfix_operator
)
{
  return new ArrayOperator(idx_list, next_postfix_operator);
}

////////////////////////////////////////////////////////////////
/// @brief FuncOperator
////////////////////////////////////////////////////////////////

FuncOperator::FuncOperator(
  std::vector<Expr *> &arguments_list_,
  PostfixOperator     *next_postfix_operator_
)
    : PostfixOperator{next_postfix_operator_}, arguments_list{arguments_list_}
{
}

void FuncOperator::print()
{
  json.print_class(" FuncOperator", [&] {
    for (auto i : arguments_list)
    {
      json.print_array_item([&] { i->print(); });
    }
  });
}

FuncOperator *FuncOperator::create(
  std::vector<Expr *> arguments_list,
  PostfixOperator    *next_postfix_operator
)
{
  return new FuncOperator(arguments_list, next_postfix_operator);
}

////////////////////////////////////////////////////////////////
/// @brief StructOperator
////////////////////////////////////////////////////////////////

StructOperator::StructOperator(
  std::string      identifier_,
  PostfixOperator *next_postfix_operator_
)
    : identifier{identifier_}, PostfixOperator{next_postfix_operator_}
{
}

void StructOperator::print()
{
  json.print_class("StructOperator", [&] {
    json.print_key_value("identifier", identifier);
  });
}

StructOperator *
StructOperator::create(std::string identifier, PostfixOperator *next_postfix_operator)
{
  return new StructOperator(identifier, next_postfix_operator);
}


////////////////////////////////////////////////////////////////
/// @brief StructOperatorPtr
////////////////////////////////////////////////////////////////

StructOperatorPtr::StructOperatorPtr(
  std::string      identifier_,
  PostfixOperator *next_postfix_operator
)
    : identifier{identifier_}, PostfixOperator{next_postfix_operator}
{
}

void StructOperatorPtr::print()
{
  json.print_class("StructOperator", [&] {
    json.print_key_value("identifier", identifier);
  });
}

StructOperatorPtr *
StructOperatorPtr::create(std::string identifier, PostfixOperator *next_postfix_operator)
{
  return new StructOperatorPtr(identifier, next_postfix_operator);
}

////////////////////////////////////////////////////////////////
/// @brief SelfPulsOperator
////////////////////////////////////////////////////////////////

SelfPulsOperator::SelfPulsOperator(PostfixOperator *next_postfix_operator)
    : PostfixOperator{next_postfix_operator}
{
}

SelfPulsOperator *SelfPulsOperator::create(PostfixOperator *next_postfix_operator)
{
  return new SelfPulsOperator(next_postfix_operator);
}
void SelfPulsOperator::print()
{
  json.print_class("SelfPulsOperator", [&] { json.print_class("", [&] {}); });
}

////////////////////////////////////////////////////////////////
/// @brief SelfMinusOperator
////////////////////////////////////////////////////////////////

SelfMinusOperator::SelfMinusOperator(PostfixOperator *next_postfix_operator)
    : PostfixOperator{next_postfix_operator}
{
}

SelfMinusOperator *SelfMinusOperator::create(PostfixOperator *next_postfix_operator)
{
  return new SelfMinusOperator(next_postfix_operator);
}

void SelfMinusOperator::print()
{
  json.print_class(" SelfMinusOperator", [] {});
}

////////////////////////////////////////////////////////////////
/// @brief PostfixOperator
////////////////////////////////////////////////////////////////
PostfixOperator::PostfixOperator(PostfixOperator *next_postfix_operator) {}

void PostfixOperator::print(){TODO}

////////////////////////////////////////////////////////////////
/// @brief InitializerNode
////////////////////////////////////////////////////////////////

InitializerNode::InitializerNode(
  Constant        *constant_,
  InitializerNode *next_initializer_node_
)
    : constant{constant_}, next_initializer_node{next_initializer_node_}
{
}

void InitializerNode::print()
{
  json.print_class("InitializerNode", [&] {
    json.print_class("constant", [&] { constant->print(); });
    json.print_class("next_initializer_node", [&] {
      if (next_initializer_node != nullptr)
      {
        next_initializer_node->print();
      }
    });
  });
}

InitializerNode *
InitializerNode::create(Constant *constant_, InitializerNode *next_initializer_node_)
{
  return new InitializerNode(constant_, next_initializer_node_);
}

////////////////////////////////////////////////////////////////
/// @brief InitializerNodeList
////////////////////////////////////////////////////////////////
InitializerNodeList::InitializerNodeList(
  InitializerNode     *initializer_node_,
  InitializerNodeList *son_initializer_node_list_,
  InitializerNodeList *next_initializer_node_list_
)
    : initializer_node{initializer_node_},
      next_initializer_node_list{next_initializer_node_list_},
      son_initializer_node_list{son_initializer_node_list_}
{
}
void InitializerNodeList::print()
{
  json.print_class("InitializerNodeList", [&] {
    if (initializer_node != nullptr)
    {
      json.print_class("initializer_node", [&] { initializer_node->print(); });
    }
    if (next_initializer_node_list != nullptr)
    {
      json.print_class("next_initializer_node", [&] {
        next_initializer_node_list->print();
      });
    }
    if (son_initializer_node_list != nullptr)
    {
      json.print_class("son_initializer_node_list", [&] {
        son_initializer_node_list->print();
      });
    }
  });
}

InitializerNodeList *InitializerNodeList::create(
  InitializerNode     *initializer_node_,
  InitializerNodeList *son_initializer_node_list_,
  InitializerNodeList *next_initializer_node_list_
)
{
  return new InitializerNodeList(
    initializer_node_, son_initializer_node_list_, next_initializer_node_list_
  );
}

////////////////////////////////////////////////////////////////
/// @brief Initializer
////////////////////////////////////////////////////////////////

Initializer::Initializer(InitializerNodeList *initializer_node_list_)
    : initializer_node_list{initializer_node_list_}
{
}

void Initializer::print()
{
  json.print_class("Initializer", [&] { initializer_node_list->print(); });
}

Initializer *Initializer::create(InitializerNodeList *initializer_node_list)
{
  return new Initializer(initializer_node_list);
}

}  // namespace spt

#endif