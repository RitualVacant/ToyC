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
  explicit Block();
  ~Block();
  static Block *create();
  uint64_t      size() const;
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
class Expr : public Base, public Statement
{
private:
  token op;
  Expr *l_expr;
  Expr *r_expr;

public:
  explicit Expr();
  Expr(token op, Expr *l_expr, Expr *r_expr);
  ~Expr();
  static Expr *create(token op, Expr *l_expr, Expr *r_expr);
};

/**
 * @brief
 *
 */

class Type
{
public:
  explicit Type();
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
  Type                    *ptr_return_type;
  std::vector<spt::Type *> argument_type_list;

public:
  explicit FuncType(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list);
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
  explicit PointerType(Type *ptr_type);
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

  explicit Func(
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
  explicit FuncDef(
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
  explicit FuncDec(
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
class FuncCall : public Expr
{
  std::string         identifier;
  std::vector<Expr *> argument_list;

private:
public:
  explicit FuncCall(std::string identifier, std::vector<Expr *> argument_list);
  ~FuncCall();
  static FuncCall *create(std::string identifier, std::vector<Expr *> argument_list);
};

class Var
{
  std::string identifier;

public:
  explicit Var(std::string identifier);
  ~Var();
  static Var *create(std::string identifier);
};


/**
 * @brief
 *
 */
class VarDef : public Def, public Var
{
  Expr *initializer;

public:
  explicit VarDef(Type *type_, std::string name_, Expr *initializer_);
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
  explicit IfStatement(Expr *expr, Block *true_block, Block *false_block);
  static IfStatement *create(Expr *expr, Block *true_block);
  static IfStatement *create(Expr *expr, Block *true_block, Block *false_block);
};

/**
 * @brief
 *
 */
class SwitchStatement : public Statement
{
};

/**
 * @brief
 *
 */
class ReturnStatement : public Statement
{
private:
  Expr *return_expr;

public:
  explicit ReturnStatement(Expr *return_expr);
  ~ReturnStatement();
  static ReturnStatement *create(Expr *return_expr);
};

/**
 * @brief
 *
 */
class WhileStatement : public Statement
{
private:
  Expr  *expr;
  Block *while_body;

public:
  explicit WhileStatement(Expr *expr, Block *while_body);
  ~WhileStatement();
  static WhileStatement *create(Expr *expr, Block *while_body);
};

/**
 * @brief
 *
 */
class DoWhileStatement : public Statement
{
private:
  Expr  *expr;
  Block *do_while_body;

public:
  explicit DoWhileStatement(Expr *expr, Block *do_while_body);
  ~DoWhileStatement();
  static DoWhileStatement *create(Expr *expr, Block *do_while_body);
};

/**
 * @brief
 *
 */
class ForStatement : public Statement
{
private:
  Block *for_body;
  Expr  *init_expr;
  Def   *init_def;
  Expr  *condition_expr;
  Expr  *change_expr;

public:
  explicit ForStatement(
    Expr  *init_expr,
    Expr  *condition_expr,
    Expr  *change_expr,
    Block *for_body
  );
  ForStatement(Def *init_def, Expr *condition_expr, Expr *change_expr, Block *for_body);
  ~ForStatement();

  static ForStatement *
  create(Expr *init_expr, Expr *condition_expr, Expr *change_expr, Block *for_body);
  static ForStatement *
  create(Def *init_def, Expr *condition_expr, Expr *change_expr, Block *for_body);
};


class Array : Expr
{
private:
  std::string         identifier;
  std::vector<Expr *> idx_list;

public:
  explicit Array(std::string identifier, std::vector<Expr *> idx_list);
  ~Array();
  static Array *create(std::string identifier, std::vector<Expr *> idx_list);
};


using spec_tree_node = std::variant<
  ArrayType,
  Array,
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