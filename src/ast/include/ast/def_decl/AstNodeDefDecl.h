//
// Created by lzj on 2023.6.5.
//

#ifndef TOYC_AST_NODE_DEF_DECL_H
#define TOYC_AST_NODE_DEF_DECL_H

#include "ast/AstNodeBasic.h"
#include "ast/AstNodeExpression.h"
#include "ast/AstNodeStatement.h"
#include "ast/def_decl/AstNodeStructOrUnion.h"
#include "ast/def_decl/TypeSpecifierAndQualifier.h"
#include <vector>

namespace toy_c::ast
{

//////////////////////////////////////////////////////////////////////////////////////////
struct TranslationUnit : public AstNode
{
};


//////////////////////////////////////////////////////////////////////////////////////////
struct StructOrUnionSpecifier;
struct DeclarationSpecifiers : public AstNode
{
  TypeSpecifier           typeSpecifier          = TypeSpecifier::_empty_;
  TypeQualifier           typeQualifier          = TypeQualifier::_empty_;
  FunctionSpecifier       functionSpecifier      = FunctionSpecifier::_empty_;
  StoreClassSpecifier     storeClassSpecifier    = StoreClassSpecifier::_empty_;
  bool                    isSigned               = true;
  Identifier             *typedefName            = nullptr;
  StructOrUnionSpecifier *structOrUnionSpecifier = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct AssignmentExpression;
struct Designator : public AstNode
{
  AssignmentExpression *assignmentExpression = nullptr;
  Identifier           *identifier           = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Designator_list : public AstNode, public std::vector<Designator *>
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Designation : public AstNode
{
  Designator_list *designatorList = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Initializer;
struct InitializerList : public AstNode,
                         public std::vector<std::tuple<Designation *, Initializer *>>
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct AssignmentExpression;
struct Initializer : public AstNode
{
  Designation          *designation          = nullptr;
  AssignmentExpression *assignmentExpression = nullptr;
  InitializerList      *initializerList      = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ArrayDeclarator : public AstNode
{
  AssignmentExpression *assignmentExpression = nullptr;
  nlohmann::json        toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ArrayDeclaratorList : public AstNode, std::vector<ArrayDeclarator *>
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct IdentifierList : public std::vector<Identifier *>, public AstNode
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Declarator;
struct ParameterDeclaration : public AstNode
{
  DeclarationSpecifiers *declarationDeclarator = nullptr;
  Declarator            *declarator            = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ParameterList : public std::vector<ParameterDeclaration *>, public AstNode
{
  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct ParameterTypeList : public AstNode
{
  ParameterList *parameterList       = nullptr;
  bool           isVariableParameter = false;

  nlohmann::json toJson() override;
};


//////////////////////////////////////////////////////////////////////////////////////////
struct DirectDeclarator : public AstNode
{
  Identifier          *identifier          = nullptr;
  ParameterTypeList   *parameterTypeList   = nullptr;
  ArrayDeclaratorList *arrayDeclaratorList = nullptr;
  Declarator          *declarator          = nullptr;
  IdentifierList      *identifierList      = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct TypeQualifierList : public AstNode, public std::vector<TypeQualifier>
{
  nlohmann::json toJson() override;
};


//////////////////////////////////////////////////////////////////////////////////////////
struct Pointer : public AstNode
{
  TypeQualifierList *typeQualifierList = nullptr;
  Pointer           *nextPointer       = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct Declarator : public AstNode
{
  DirectDeclarator *directDeclarator = nullptr;
  Pointer          *pointer          = nullptr;

  nlohmann::json toJson() override;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct InitialDeclarator : public AstNode
{
  Declarator  *declarator  = nullptr;
  Initializer *initializer = nullptr;

  nlohmann::json toJson() override;
};

struct InitialDeclaratorList : public AstNode, public std::vector<InitialDeclarator *>
{
  nlohmann::json toJson() override;
};


struct CompoundStatement;
struct DeclarationOrDefinition : public AstNode
{
  DeclarationSpecifiers *declarationDeclarator = nullptr;
  InitialDeclaratorList *initialDeclaratorList = nullptr;
  CompoundStatement     *compoundStatement     = nullptr;
  //  NextDeclarationOrDefinition *nextDeclarationOrDefinition = nullptr;

  nlohmann::json toJson() override;
};


}  // namespace toy_c::ast


#endif  // TOYC_AST_NODE_DEF_DECL_H
