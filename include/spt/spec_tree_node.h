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
 * @brief Base
 *
 */
class Base
{
private:
  std::size_t line   = 0;
  std::size_t column = 0;

public:
  virtual void print();
  Base()  = default;
  ~Base() = default;
};

/**
 * @brief Statement
 *
 */
class Statement : public Base
{
public:
  void print() override;
};

/**
 * @brief Block
 *
 */
class Block : public Base
{
private:
  std::vector<Statement *> block_body;

public:
  explicit Block();
  ~Block();
  static Block *create();
  uint64_t      size() const;
  void          push_back(Statement *ptr_statement);
  void          push_back(Block *block);
  void          print() override;
};

/**
 * @brief Def
 *
 */
class Def : public Statement
{
public:
  void print() override;
};

/**
 * @brief Dec
 *
 */
class Dec : public Statement
{
public:
  void print() override;
};


/**
 * @brief Expr
 *
 */
class Expr : public Statement
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
  void         print() override;
};

/**
 * @brief Type
 *
 */

class Type : public Base
{
private:
public:
  explicit Type();
  ~Type();
  // offset
  // 0
  static Type *get_int8();
  // 1
  static Type *get_int16();
  // 2
  static Type *get_int32();
  // 3
  static Type *get_int64();
  // 4
  static Type *get_uint8();
  // 5
  static Type *get_uint16();
  // 6
  static Type *get_uint32();
  // 7
  static Type *get_uint64();
  // 8
  static Type *get_void();
  // 9
  static Type *get_struct();
  // 10
  static Type *get_double();
  // 11
  static Type *get_float();
  void         print() override;
};

/**
 * @brief FuncType
 *
 */
class FuncType : public Type
{
  Type                    *ptr_return_type;
  std::vector<spt::Type *> argument_type_list;

public:
  explicit FuncType(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list);
  ~FuncType();
  static FuncType      *
  get(Type *ptr_return_type, std::vector<spt::Type *> argument_type_list);
  void print() override;
};

/**
 * @brief ArrayType
 *
 */
class ArrayType : public Type
{
private:
  std::vector<std::uint64_t> dimension_len;
  Type                      *ptr_unit_type;

public:
  static ArrayType *get(Type *ptr_unit_type, std::vector<std::uint64_t> dimension_len);
  void              print() override;
};

/**
 * @brief PointerType
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
  void print() override;
};


/**
 * @brief Func
 *
 */
class Func : public Base
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
  void print() override;
};

/**
 * @brief FuncDef
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
  //

  virtual void print() override;
};

/**
 * @brief FuncDec
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
  virtual void print() override;
};

/**
 * @brief FuncCall
 *
 */
class FuncCall : public Expr
{
protected:
  std::string         identifier;
  std::vector<Expr *> argument_list;

public:
  explicit FuncCall(std::string identifier, std::vector<Expr *> argument_list);
  ~FuncCall();
  static FuncCall *create(std::string identifier, std::vector<Expr *> argument_list);
  void             print() override;
};

/**
 * @brief Var
 *
 */

class Var : public Base
{
protected:
  std::string identifier;
  Type       *type;

public:
  explicit Var(std::string identifier_, Type *type_);
  ~Var();
  static Var *create(std::string identifier_, Type *type_);
  void        print() override;
};


/**
 * @brief VarDef
 *
 */
class VarDef : public Def, public Var
{
private:
  Expr *initializer;

public:
  explicit VarDef(Type *type_, std::string identifier_, Expr *initializer_);
  static VarDef      *
  create(Type *type_, std::string identifier_, Expr *initializer_ = nullptr);
  void print() override;
};

/**
 * @brief VarDec
 *
 */
class VarDec : public Dec, public Var
{
public:
  void print() override;
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
  void print() override;
};

/**
 * @brief
 *
 */
class StructDec : public Dec
{
public:
  static StructDec *create(std::string name);
  void              print() override;
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
  void                print() override;
};

/**
 * @brief
 *
 */
class SwitchStatement : public Statement
{
public:
  void print() override;
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
  void                    print() override;
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
  void                   print() override;
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
  void                     print() override;
};

/**
 * @brief ForStatement
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
  static ForStatement      *
  create(Def *init_def, Expr *condition_expr, Expr *change_expr, Block *for_body);
  void print() override;
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
  void          print() override;
};


// DROP
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