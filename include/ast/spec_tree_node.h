#ifndef SPEC_TREE_NODE_H
#define SPEC_TREE_NODE_H

#include "inner.h"
#include <limits>
#include <string>
#include <variant>
#include <vector>


namespace spt
{

class Type;
class Dec;
class Def;
class FuncDec;
class FuncDef;
class VarDec;
class VarDef;

using spec_tree_node = std::variant<
  Dec,
  Def,
  FuncDec,
  FuncDef,
  VarDec,
  VarDef

  >;

std::vector<spec_tree_node> *spec_tree = nullptr;

extern Type basic_type_loc[20];


// class Base;
// class Def;
// class Dec;
// class Value;
// class Func;
// class Expr;
// class Type;
// class ArrayType;
// class FuncDef;
// class FuncDec;
// class FuncCall;
// class VarDef;
// class VarDec;
// class StrcutDef;
// class StructDec;

/**
 * @brief
 *
 */
class Block
{
};

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
class Def : public Base
{
  std::string identifier;
};

/**
 * @brief
 *
 */
class Dec : public Base
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
class Expr : public Base
{
};

/**
 * @brief
 *
 */

class Type
{
public:
  static Type *getInt8Ty()
  {
    return &basic_type_loc[0];
  }
  static Type *getDoubleTy()
  {
    return &basic_type_loc[1];
  }
  static Type *getFloatTy()
  {
    return &basic_type_loc[2];
  }
  static Type *getInt16Ty()
  {
    return &basic_type_loc[3];
  }
  static Type *getInt32Ty()
  {
    return &basic_type_loc[4];
  };
  static Type *getInt64Ty()
  {
    return &basic_type_loc[5];
  };
  static Type *getVoid()
  {
    return &basic_type_loc[6];
  };
  static Type *getStructTy()
  {
    return &basic_type_loc[7];
  };
  void dump(){

  };
};
/**
 * @brief
 *
 */
class FunctionType : public Type
{
public:
  static FunctionType *get(
    Type                                                          *ptr_return_type,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list
  ){

  };
};

/**
 * @brief
 *
 */
class ArrayType : public Type
{
  std::vector<std::uint64_t> dimension_len;
  Type                      *ptr_unit_type;

public:
  static ArrayType *get(Type *ptr_unit_type, std::vector<std::uint64_t> dimension_len){
    TODO
    // this->ptr_unit_type = ptr_unit_type;
    // this->dimension_len = dimension_len
  };
};


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

  )
      : ptr_return_type{ptr_return_type_}, name{name_}, argument_type_list{
                                                          argument_type_list_} {};
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
  )
      : Func(ptr_return_type_, name_, argument_type_list_), body{body_} {};

  static FuncDef *create(
    Type                                                          *ptr_return_type_,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list_,
    std::string                                                    name_,
    Block                                                         *body_
  ){

  };
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
  )
      : Func(ptr_return_type_, name_, argument_type_list_){};
  static FuncDec *create(
    Type                                                          *ptr_return_type,
    std::string                                                    name,
    std::tuple<std::vector<spt::Type *>, std::vector<std::string>> argument_type_list
  ){

  };
};

/**
 * @brief
 *
 */
class FuncCall : public Value
{
public:
};

class Var : public Def
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
  VarDef(Type *type_, std::string name_, Expr *initializer_)
      : Var(), initializer{initializer_} {};

  static VarDef *create(Type *type_, std::string name_, Expr *initializer_ = nullptr){

  };
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
  ){};
};

/**
 * @brief
 *
 */
class StructDec : public Dec
{
public:
  static StructDec *create(std::string name){};
};


}  // namespace spt

#endif