//
// Created by lzj on 2023.6.22.
//

#ifndef TOYC_AST_NODE_STRUCT_OR_UNION_H
#define TOYC_AST_NODE_STRUCT_OR_UNION_H

#include "ast/AstNodeBasic.h"
#include "ast/AstNodeExpression.h"
#include "ast/def_decl/AstNodeDefDecl.h"
#include "ast/def_decl/TypeSpecifierAndQualifier.h"
#include <vector>

namespace toy_c::ast
{


//////////////////////////////////////////////////////////////////////////////////////////
struct Declarator;
struct AssignmentExpression;
struct StructDeclarator : public AstNode
{
  Declarator           *declarator           = nullptr;
  AssignmentExpression *assignmentExpression = nullptr;
  nlohmann::json        toJson() override;
};


//////////////////////////////////////////////////////////////////////////////////////////
struct StructDeclaratorList : public AstNode, public std::vector<StructDeclarator *>
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct SpecifierQualifierList : public AstNode
{
  std::vector<TypeQualifier> typeQualifierList;
  TypeSpecifier              typeSpecifier = TypeSpecifier::_empty_;
  bool                       isSigned      = true;
  nlohmann::json             toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct StructDeclaration : public AstNode
{
  SpecifierQualifierList *specifierQualifierList = nullptr;
  StructDeclaratorList   *structDeclaratorList   = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct StructDeclarationList : public AstNode, public std::vector<StructDeclaration *>
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct StructOrUnionSpecifier : public AstNode
{
  bool                   isStruct              = true;
  Identifier            *identifier            = nullptr;
  StructDeclarationList *structDeclarationList = nullptr;

  nlohmann::json toJson() override;
};

}  // namespace toy_c::ast


#endif  // TOYC_AST_NODE_STRUCT_OR_UNION_H
