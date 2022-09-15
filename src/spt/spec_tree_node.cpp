#ifndef SPEC_TREE_NODE_CPP
#define SPEC_TREE_NODE_CPP
#include "spec_tree_node.h"
#include <iostream>

#define PRINT_SIZE std::cout << "tree size:" << ptr_tree_body->size() << std::endl;

namespace spt
{
////////////////////////////////////////////////////////////////
/// @brief Block
////////////////////////////////////////////////////////////////

Block::Block() {}
Block::~Block() {}
Block *Block::create()
{
  ptr_tree_body->push_back(Block());
  return &std::get<Block>(ptr_tree_body->back());
}

void Block::push_back(Statement *ptr_statement)
{
  block_body.push_back(ptr_statement);
}

uint64_t Block::size() const
{
  return block_body.size();
}

////////////////////////////////////////////////////////////////
/// @brief Type
////////////////////////////////////////////////////////////////
Type basic_type_loc[20];

Type::Type() {}
Type::~Type() {}

Type *Type::getInt8Ty()
{
  return &basic_type_loc[0];
}

Type *Type::getDoubleTy()
{
  return &basic_type_loc[1];
}

Type *Type::getFloatTy()
{
  return &basic_type_loc[2];
}
Type *Type::getInt16Ty()
{
  return &basic_type_loc[3];
}
Type *Type::getInt32Ty()
{
  return &basic_type_loc[4];
};
Type *Type::getInt64Ty()
{
  return &basic_type_loc[5];
};
Type *Type::getVoid()
{
  return &basic_type_loc[6];
};
Type *Type::getStructTy()
{
  return &basic_type_loc[7];
};
void Type::dump(){

};

////////////////////////////////////////////////////////////////
/// @brief FunctionType
////////////////////////////////////////////////////////////////
FuncType::FuncType(Type *ptr_return_type_, std::vector<spt::Type *> argument_type_list_)
    : ptr_return_type{ptr_return_type_}, argument_type_list{argument_type_list_}
{
}

FuncType::~FuncType() {}

FuncType *
FuncType::get(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list)
{
  FuncType f(ptr_return_type, argument_type_list);
  // TODO this step will write memory belong to block located in 0x555555a1f550
  PRINT_SIZE
  ptr_tree_body->emplace_back(f);
  PRINT_SIZE
  std::cout << &std::get<FuncType>(ptr_tree_body->back()) << std::endl;
  return &std::get<FuncType>(ptr_tree_body->back());
}


////////////////////////////////////////////////////////////////
/// @brief ArrayType
////////////////////////////////////////////////////////////////

ArrayType *
ArrayType::get(Type *ptr_unit_type, std::vector<std::uint64_t> dimension_len){};

////////////////////////////////////////////////////////////////
/// @brief PointerType
////////////////////////////////////////////////////////////////

PointerType *PointerType::get(Type *ptr_type_)
{
  ptr_tree_body->push_back(PointerType(ptr_type_));
  PRINT_SIZE
  return &std::get<PointerType>(ptr_tree_body->back());
}

PointerType::PointerType(Type *ptr_type_) : ptr_element_type{ptr_type_} {}
PointerType::~PointerType() {}

////////////////////////////////////////////////////////////////
/// @brief Func
////////////////////////////////////////////////////////////////

Func::Func(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_

)
    : ptr_return_type{ptr_return_type_}, name{name_}, argument_type_list{
                                                        argument_type_list_} {};
Func::~Func(){};

////////////////////////////////////////////////////////////////
/// @brief FuncDef
////////////////////////////////////////////////////////////////


FuncDef::FuncDef(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
  Block                                                         *body_
)
    : Func(ptr_return_type_, name_, argument_type_list_), body{body_} {};

FuncDef *FuncDef::create(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
  Block                                                         *body_
)
{
  ptr_tree_body->push_back(FuncDef(ptr_return_type_, name_, argument_type_list_, body_));
  return &std::get<FuncDef>(ptr_tree_body->back());
}

////////////////////////////////////////////////////////////////
/// @brief FuncDec
////////////////////////////////////////////////////////////////

FuncDec::FuncDec(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_
)
    : Func(ptr_return_type_, name_, argument_type_list_)
{
}

FuncDec *FuncDec::create(
  Type                                                          *ptr_return_type_,
  std::string                                                    name_,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_
)
{
  ptr_tree_body->push_back(FuncDec(ptr_return_type_, name_, argument_type_list_));
  return &std::get<FuncDec>(ptr_tree_body->back());
}

////////////////////////////////////////////////////////////////
/// @brief VarDef
////////////////////////////////////////////////////////////////

VarDef::VarDef(Type *type_, std::string name_, Expr *initializer_)
    : Var{name_}, initializer{initializer_}
{
}

VarDef *VarDef::create(Type *type_, std::string name_, Expr *initializer_)
{
  ptr_tree_body->push_back(VarDef(type_, name_, initializer_));
  return &std::get<VarDef>(ptr_tree_body->back());
}

////////////////////////////////////////////////////////////////
/// @brief StrcutDef
////////////////////////////////////////////////////////////////

StructDef *StructDef::create(
  std::string                                                    name,
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> element_list
){TODO}

////////////////////////////////////////////////////////////////
/// @brief StructDec
////////////////////////////////////////////////////////////////

StructDec *StructDec::create(std::string name){TODO};

////////////////////////////////////////////////////////////////
/// @brief IfStatement
////////////////////////////////////////////////////////////////

IfStatement::IfStatement(Expr *expr, Block *true_block, Block *false_block)
    : expr{expr}, true_block{true_block}, false_block{false_block}
{
}

IfStatement *IfStatement::create(Expr *expr, Block *true_block, Block *false_block)
{
  ptr_tree_body->push_back(IfStatement(expr, true_block, false_block));
  return &std::get<IfStatement>(ptr_tree_body->back());
}

IfStatement *IfStatement::create(Expr *expr, Block *true_block)
{
  ptr_tree_body->push_back(IfStatement(expr, true_block, nullptr));
  return &std::get<IfStatement>(ptr_tree_body->back());
}

////////////////////////////////////////////////////////////////
/// @brief ReturnStatement
////////////////////////////////////////////////////////////////

ReturnStatement::ReturnStatement(Expr *return_expr_) : return_expr{return_expr_} {}
ReturnStatement::~ReturnStatement() {}
ReturnStatement *ReturnStatement::create(Expr *return_expr)
{
  ptr_tree_body->push_back(ReturnStatement(return_expr));
  return &std::get<ReturnStatement>(ptr_tree_body->back());
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
  ptr_tree_body->push_back(WhileStatement(expr, while_body));
  return &std::get<WhileStatement>(ptr_tree_body->back());
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
  ptr_tree_body->push_back(DoWhileStatement(expr, do_while_body));
  return &std::get<DoWhileStatement>(ptr_tree_body->back());
}


////////////////////////////////////////////////////////////////
/// @brief ForStatement
////////////////////////////////////////////////////////////////

ForStatement::ForStatement(
  Expr  *init_expr_,
  Expr  *condition_expr_,
  Expr  *change_expr_,
  Block *for_body_
)
    : init_expr(init_expr_), condition_expr{condition_expr_},
      change_expr{change_expr_}, for_body{for_body_}
{
}
ForStatement::ForStatement(
  Def   *init_def_,
  Expr  *condition_expr_,
  Expr  *change_expr_,
  Block *for_body_
)
    : init_def{init_def_}, condition_expr{condition_expr_},
      change_expr{change_expr_}, for_body{for_body_}
{
}

ForStatement::~ForStatement() {}

ForStatement *ForStatement::create(
  Expr  *init_expr,
  Expr  *condition_expr,
  Expr  *change_expr,
  Block *for_body
){TODO}

ForStatement *ForStatement::create(
  Def   *init_def,
  Expr  *condition_expr,
  Expr  *change_expr,
  Block *for_body
){TODO}


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
  ptr_tree_body->push_back(Expr(op, l_expr, r_expr));
  return &std::get<Expr>(ptr_tree_body->back());
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
  ptr_tree_body->push_back(Array(identifier, idx_list));
  return &std::get<Array>(ptr_tree_body->back());
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
  ptr_tree_body->push_back(FuncCall(identifier, argument_list));
  return &std::get<FuncCall>(ptr_tree_body->back());
}


////////////////////////////////////////////////////////////////
/// @brief FuncCall
////////////////////////////////////////////////////////////////

Var::Var(std::string identifier_) : identifier{identifier_} {}

Var::~Var() {}

Var *Var::create(std::string identifier)
{
  ptr_tree_body->push_back(Var(identifier));
  return &std::get<Var>(ptr_tree_body->back());
}


////////////////////////////////////////////////////////////////
/// @brief FuncCall
////////////////////////////////////////////////////////////////


}  // namespace spt

#endif