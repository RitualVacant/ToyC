#ifndef SPEC_TREE_NODE_H
#define SPEC_TREE_NODE_H

#include "inner.h"
#include <limits>
#include <string>
#include <variant>
#include <vector>

namespace spt
{

/**
 * @brief
 *
 */
class Base
{
  std::size_t line;
  std::size_t column;
};

/**
 * @brief
 *
 */
class Statement
{
};

/**
 * @brief
 *
 */
class Block
{
  std::vector<Statement *> block_body;

public:
  Block();
  ~Block();
  static Block *create();
  void          push_back(Statement *ptr_statement);
};

/**
 * @brief
 *
 */
class Def : public Base, public Statement
{
};

/**
 * @brief
 *
 */
class Dec : public Base, public Statement
{
};

/**
 * @brief
 *
 */
class Value : public Base
{
  std::string identifier;
};


/**
 * @brief
 *
 */
class Expr : public Base, public Statement
{
};

/**
 * @brief
 *
 */

class Type
{
public:
  Type *type = nullptr;
  Type();
  ~Type();
  static Type *getInt8Ty();
  static Type *getDoubleTy();
  static Type *getFloatTy();
  static Type *getInt16Ty();
  static Type *getInt32Ty();
  static Type *getInt64Ty();
  static Type *getVoid();
  static Type *getStructTy();
  void         dump();
};

/**
 * @brief
 *
 */
class FuncType : public Type
{
public:
  Type                    *ptr_return_type;
  std::vector<spt::Type *> argument_type_list;
  FuncType(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list);
  ~FuncType();
  static FuncType *
  get(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list);
};

/**
 * @brief
 *
 */
class ArrayType : public Type
{
private:
  std::vector<std::uint64_t> dimension_len;
  Type                      *ptr_unit_type;

public:
  static ArrayType *get(Type *ptr_unit_type, std::vector<std::uint64_t> dimension_len);
};

/**
 * @brief
 *
 */

class PointerType : public Type
{
private:
  Type *ptr_element_type;

public:
  static PointerType *get(Type *ptr_type);
  PointerType(Type *ptr_type);
  ~PointerType();
};


/**
 * @brief
 *
 */
class Func
{
public:
  std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list;

  Type       *ptr_return_type;
  std::string name;

  Func(
    Type                                                          *ptr_return_type_,
    std::string                                                    name_,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_

  );
  ~Func();
};

/**
 * @brief
 *
 */
class FuncDef : public Def, public Func
{
private:
  Block *body;

public:
  FuncDef(
    Type                                                          *ptr_return_type_,
    std::string                                                    name_,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
    Block                                                         *body_
  );

  static FuncDef *create(
    Type                                                          *ptr_return_type_,
    std::string                                                    name_,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
    Block                                                         *body_
  );
};

/**
 * @brief
 *
 */
class FuncDec : public Dec, public Func
{
private:
public:
  FuncDec(
    Type                                                          *ptr_return_type_,
    std::string                                                    name_,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_
  );
  static FuncDec *create(
    Type                                                          *ptr_return_type,
    std::string                                                    name,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list
  );
};

/**
 * @brief
 *
 */
class FuncCall : public Value
{
public:
};

class Var
{
public:
  Type       *type;
  std::string name;
};


/**
 * @brief
 *
 */
class VarDef : public Def, public Var
{
  Expr *initializer;

public:
  VarDef(Type *type_, std::string name_, Expr *initializer_);
  static VarDef *create(Type *type_, std::string name_, Expr *initializer_ = nullptr);
};

/**
 * @brief
 *
 */
class VarDec : public Dec
{
};

/**
 * @brief
 *
 */
class StructDef : public Def
{
  std::tuple<std::vector<llvm::Type *>, std::vector<std::string>> element_list;

public:
  static StructDef *create(
    std::string                                                    name,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> element_list
  );
};

/**
 * @brief
 *
 */
class StructDec : public Dec
{
public:
  static StructDec *create(std::string name);
};


/**
 * @brief
 *
 */
class IfStatement : public Statement
{
private:
  Expr  *expr;
  Block *true_block;
  Block *false_block;

public:
  IfStatement(Expr *expr, Block *true_block, Block *false_block);
  static IfStatement *create(Expr *expr, Block *true_block);
  static IfStatement *create(Expr *expr, Block *true_block, Block *false_block);
};

class SwitchStatement : public Statement
{
};

class ReturnStatement : public Statement
{
private:
  Expr *return_expr;

public:
  ReturnStatement(Expr *return_expr);
  ~ReturnStatement();
  static ReturnStatement *create(Expr *return_expr);
};

class WhileStatement : public Statement
{
private:
  Expr  *expr;
  Block *while_body;

public:
  WhileStatement(Expr *expr, Block *while_body);
  ~WhileStatement();
  static WhileStatement *create(Expr *expr, Block *while_body);
};

class DoWhileStatement : public Statement
{
private:
  Expr  *expr;
  Block *do_while_body;

public:
  DoWhileStatement(Expr *expr, Block *do_while_body);
  ~DoWhileStatement();
  static DoWhileStatement *create(Expr *expr, Block *do_while_body);
};

class ForStatement : public Statement
{
private:
  Block *for_body;
  Expr  *init_expr;
  Def   *init_def;
  Expr  *condition_expr;
  Expr  *change_expr;

public:
  ForStatement(Expr *init_expr, Expr *condition_expr, Expr *change_expr, Block *for_body);
  ForStatement(Def *init_def, Expr *condition_expr, Expr *change_expr, Block *for_body);
  ~ForStatement();

  static ForStatement *
  create(Expr *init_expr, Expr *condition_expr, Expr *change_expr, Block *for_body);
  static ForStatement *
  create(Def *init_def, Expr *condition_expr, Expr *change_expr, Block *for_body);
};


using spec_tree_node = std::variant<
  ArrayType,
  Base,
  Block,
  Dec,
  Def,
  Expr,
  Func,
  FuncType,
  FuncCall,
  FuncDec,
  FuncDef,
  Type,
  PointerType,
  Value,
  Var,
  VarDef,
  VarDec,
  Statement,
  IfStatement,
  SwitchStatement,
  ReturnStatement,
  WhileStatement,
  DoWhileStatement,
  ForStatement>;

extern std::vector<spec_tree_node> *ptr_tree_body;

}  // namespace spt


#endif