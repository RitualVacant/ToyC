#ifndef SPEC_TREE_NODE_H
#define SPEC_TREE_NODE_H

#include "inner.h"
#include "token.h"
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
  virtual void gen();
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
  void gen() override;
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
  void          gen() override;
};

/**
 * @brief Def
 *
 */
class Def : public Statement
{
public:
  void print() override;
  void gen() override;
};

/**
 * @brief Dec
 *
 */
class Dec : public Statement
{
public:
  void print() override;
  void gen() override;
};


/**
 * @brief Expr
 *
 */
class Expr : public Statement
{
private:
  token op;
  Expr *l_expr = nullptr;
  Expr *r_expr = nullptr;

public:
  explicit Expr();
  Expr(token op, Expr *l_expr, Expr *r_expr);
  ~Expr();
  static Expr *create(token op, Expr *l_expr, Expr *r_expr);
  void         print() override;
  void         gen() override;
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
  // 12
  static Type *get_udouble();
  // 13
  static Type *get_ufloat();

  void print() override;
  void gen() override;
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
  void gen() override;
};

/**
 * @brief ArrayType
 *
 */
class ArrayType : public Type
{
private:
  std::vector<std::uint64_t> dimension_len;
  std::vector<Expr *>        dimension_len_expr;
  Type                      *ptr_unit_type;
  ArrayType(Type *ptr_unit_type, std::vector<Expr *> dimension_len_expr);

public:
  static ArrayType *get(Type *ptr_unit_type, std::vector<Expr *> dimension_len_expr);
  void              print() override;
  void              gen() override;
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
  void gen() override;
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
  void gen() override;
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
  void         gen() override;
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
  void         gen() override;
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
  void             gen() override;
};


/**
 * @brief
 *
 */
class Constant : public Expr
{
protected:
  std::string value;

public:
  Constant(std::string value);
  ~Constant() = default;
  void print() override;
  void gen() override;
};

/**
 * @brief
 *
 */
class ConstantInt : public Constant
{
private:
  Type *type;
  bool  negative = false;

public:
  ConstantInt(std::string value, bool negative);
  ~ConstantInt() = default;
  static ConstantInt *create(std::string value, bool negative);
  void                print() override;
  void                gen() override;
};

/**
 * @brief
 *
 */
class ConstantFloat : public Constant
{
private:
  Type *type;
  bool  negative = false;

public:
  ConstantFloat(std::string value, bool negative);
  ~ConstantFloat() = default;
  static ConstantFloat *create(std::string value, bool negative);
  void                  print() override;
  void                  gen() override;
};

/**
 * @brief
 *
 */
class StringLiteral : public Constant
{
public:
  StringLiteral(std::string value);
  ~StringLiteral() = default;
  static StringLiteral *create(std::string value);
  void                  print() override;
  void                  gen() override;
};


/**
 * @brief Conversion
 *
 */
class Conversion : public Expr
{
private:
  Type *type;
  Expr *expr;

public:
  explicit Conversion(Type *type, Expr *expr);
  static Conversion *create(Type *type, Expr *expr);
  void               print() override;
  void               gen() override;
};

/**
 * @brief
 *
 */
class UnaryExpr : public Expr
{
private:
  token unary_op;
  Expr *expr;

public:
  explicit UnaryExpr(token unary_op, Expr *expr);
  static UnaryExpr *create(token unary_op, Expr *expr);
  void              print() override;
  void              gen() override;
};


class Array : public Expr
{
private:
  std::string         identifier;
  std::vector<Expr *> idx_list;

public:
  explicit Array(std::string identifier, std::vector<Expr *> idx_list);
  ~Array();
  static Array *create(std::string identifier, std::vector<Expr *> idx_list);
  void          print() override;
  void          gen() override;
};


/**
 * @brief InitializerNode
 *
 */
class InitializerNode
{
public:
  Constant        *constant              = nullptr;
  InitializerNode *next_initializer_node = nullptr;
  InitializerNode(Constant *constant_, InitializerNode *next_initializer_node_);
  static InitializerNode      *
  create(Constant *constant_, InitializerNode *next_initializer_node_);
  void print();
  void gen();
};

/**
 * @brief InitializerNodeList
 *
 */
class InitializerNodeList
{
public:
  InitializerNode     *initializer_node           = nullptr;
  InitializerNodeList *next_initializer_node_list = nullptr;
  InitializerNodeList *son_initializer_node_list  = nullptr;
  InitializerNodeList(
    InitializerNode     *initializer_node_,
    InitializerNodeList *son_initializer_node_list_,
    InitializerNodeList *next_initializer_node_list_
  );
  static InitializerNodeList *create(
    InitializerNode     *initializer_node_,
    InitializerNodeList *son_initializer_node_list_,
    InitializerNodeList *next_initializer_node_list_
  );
  void print();
  void gen();
};

/**
 * @brief Initializer
 *
 */
class Initializer
{
private:
  InitializerNodeList *initializer_node_list;
  Initializer(InitializerNodeList *initializer_node_list);

public:
  void                print();
  void                gen();
  static Initializer *create(InitializerNodeList *initializer_node_list);
};

/**
 * @brief ArrayDef
 *
 */
class ArrayDef : public Statement
{
private:
  Type        *array_type;
  std::string  identifier;
  Initializer *initializer;
  ArrayDef(Type *array_type, std::string identifier, Initializer *initializer);

public:
  static ArrayDef      *
  create(Type *array_type, std::string identifier, Initializer *initializer);
  void print() override;
  void gen() override;
};

/**
 * @brief ArrayDecl
 *
 */
class ArrayDecl : public Statement
{
private:
  Type        *array_type;
  std::string  identifier;
  Initializer *initializer;
  ArrayDecl(Type *array_type, std::string identifier, Initializer *initializer);

public:
  static ArrayDecl      *
  create(Type *array_type, std::string identifier, Initializer *initializer);
  void print() override;
  void gen() override;
};

/**
 * @brief StructDef
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
  void gen() override;
};

/**
 * @brief StructDec
 *
 */
class StructDec : public Dec
{
public:
  static StructDec *create(std::string name);
  void              print() override;
  void              gen() override;
};

/**
 * @brief IfStatement
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
  void                gen() override;
};

/**
 * @brief
 *
 */
class SwitchStatement : public Statement
{
public:
  void print() override;
  void gen() override;
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
  void                    gen() override;
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
  void                   gen() override;
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
  void                     gen() override;
};

/**
 * @brief ForStatement
 *
 */
class ForStatement : public Statement
{
private:
  Block *init_block;
  Expr  *condition_expr;
  Expr  *change_expr;
  Block *for_body;

public:
  explicit ForStatement(
    Block *init_block,
    Expr  *condition_expr,
    Expr  *change_expr,
    Block *for_body
  );
  ~ForStatement();

  static ForStatement *
  create(Block *init_block, Expr *condition_expr, Expr *change_expr, Block *for_body);
  void print() override;
  void gen() override;
};

////////////////////////////////////////
// postfix operators
////////////////////////////////////////

/**
 * @brief PostfixOperator
 *
 */
class PostfixOperator : public Base
{
protected:
  PostfixOperator *next_postfix_operator = nullptr;

public:
  PostfixOperator(PostfixOperator *next_postfix_operator);
  virtual void print();
  void         gen() override;
};

/**
 * @brief ArrayOperator
 *
 */
class ArrayOperator : public PostfixOperator
{
private:
  std::vector<Expr *> idx_list;

  ArrayOperator(std::vector<Expr *> &idx_list, PostfixOperator *next_postfix_operator);

public:
  void print() override;
  void gen() override;
  static ArrayOperator *
  create(std::vector<Expr *> &idx_list, PostfixOperator *next_postfix_operator);
};

/**
 * @brief
 *
 */
class FuncOperator : public PostfixOperator
{
private:
  std::vector<Expr *> arguments_list;

  FuncOperator(
    std::vector<Expr *> &arguments_list,
    PostfixOperator     *next_postfix_operator
  );

public:
  void print() override;
  void gen() override;
  static FuncOperator *
  create(std::vector<Expr *> arguments_list, PostfixOperator *next_postfix_operator);
};

/**
 * @brief StructOperator
 *
 */
class StructOperator : public PostfixOperator
{
private:
  std::string identifier;
  StructOperator(std::string identifier, PostfixOperator *next_postfix_operator);

public:
  void print() override;
  void gen() override;
  static StructOperator *
  create(std::string identifier, PostfixOperator *next_postfix_operator);
};

/**
 * @brief StructOperatorPtr
 *
 */
class StructOperatorPtr : public PostfixOperator
{
  std::string identifier;
  StructOperatorPtr(std::string identifier, PostfixOperator *next_postfix_operator);

public:
  void print() override;
  void gen() override;
  static StructOperatorPtr *
  create(std::string identifier, PostfixOperator *next_postfix_operator);
};

/**
 * @brief SelfPulsOperator
 *
 */
class SelfPulsOperator : public PostfixOperator
{
private:
  SelfPulsOperator(PostfixOperator *next_postfix_operator);

public:
  static SelfPulsOperator *create(PostfixOperator *next_postfix_operator);
  void                     print() override;
  void                     gen() override;
};

/**
 * @brief SelfMinusOperator
 *
 */
class SelfMinusOperator : public PostfixOperator
{
private:
  SelfMinusOperator(PostfixOperator *next_postfix_operator);

public:
  static SelfMinusOperator *create(PostfixOperator *next_postfix_operator);
  void                      print() override;
  void                      gen() override;
};


/**
 * @brief Var
 *
 */

class Var : public Expr
{
private:
  PostfixOperator *next_postfix_operator;

protected:
  std::string identifier;
  Type       *type;

public:
  explicit Var(
    std::string      identifier_,
    Type            *type_,
    PostfixOperator *next_postfix_operator
  );
  ~Var();
  static Var *create(
    std::string      identifier_,
    Type            *type_,
    PostfixOperator *next_postfix_operator = nullptr
  );
  void print() override;
  void gen() override;
};


/**
 * @brief VarDef
 *
 */
class VarDef : public Def
{
private:
  std::string identifier;
  Type       *type;
  Expr       *initializer;

  explicit VarDef(Type *type_, std::string identifier_, Expr *initializer_);

public:
  static VarDef      *
  create(Type *type_, std::string identifier_, Expr *initializer_ = nullptr);
  void print() override;
  void gen() override;
};

/**
 * @brief VarDec
 *
 */
class VarDec : public Dec
{
  std::string identifier;
  Type       *type;

  explicit VarDec(Type *type_, std::string identifier_, Expr *initializer_);

public:
  static VarDef      *
  create(Type *type_, std::string identifier_, Expr *initializer_ = nullptr);
  void print() override;
  void gen() override;
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