#ifndef SPEC_TREE_NODE_H
#define SPEC_TREE_NODE_H

#include "inner.h"
#include <limits>
#include <string>
#include <vector>


namespace ast
{

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
class Func : public Base
{
  std::string identifier;
};

/**
 * @brief
 *
 */
class Expr
{
};

/**
 * @brief
 *
 */

class Type
{
public:
  static Type *get(){};
  static Type *getInt8Ty(){};
  static Type *getDoubleTy(){};
  static Type *getFloatTy(){};
  static Type *getInt16Ty(){};
  static Type *getInt32Ty(){};
  static Type *getInt64Ty(){};
  static Type *getVoid(){};

  void dump(){

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

/**
 * @brief
 *
 */
class FunctionType : public Type
{
public:
  static FunctionType *get(
    Type                                                          *ptr_return_type,
    std::tuple<std::vector<ast::Type *>, std::vector<std::string>> argument_type_list
  ){};
};

/**
 * @brief
 *
 */
class FuncDef : public Def
{
private:
  std::tuple<std::vector<ast::Type *>, std::vector<std::string>> argument_type_list;
  Type                                                          *ptr_return_type;
  std::string                                                    name;

public:
  static FuncDef *create(
    Type                                                          *ptr_return_type,
    std::tuple<std::vector<ast::Type *>, std::vector<std::string>> argument_type_list,
    std::string                                                    name,
    Block                                                         *body
  ){};
};

/**
 * @brief
 *
 */
class FuncDec : public Dec
{
private:
  std::tuple<std::vector<ast::Type *>, std::vector<std::string>> argument_type_list;
  Type                                                          *ptr_return_type;
  std::string                                                    name;

public:
  create(
    Type                                                          *ptr_return_type,
    std::tuple<std::vector<ast::Type *>, std::vector<std::string>> argument_type_list,
    std::string                                                    name
  ){};
};

/**
 * @brief
 *
 */
class FuncCall : public Value
{
};

/**
 * @brief
 *
 */
class VarDef : public Def
{
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
class StrcutDef : public Def
{
  std::tuple<std::vector<llvm::Type *>, std::vector<std::string>> element_list;

public:
  static create(
    std::tuple<std::vector<llvm::Type *>, std::vector<std::string>> element_list,
    std::string                                                     name
  ){};
};

/**
 * @brief
 *
 */
class StructDec : public Dec
{
  static create(std::string name){};
};

/**
 * @brief
 *
 */
class Block
{
};

}  // namespace ast

#endif