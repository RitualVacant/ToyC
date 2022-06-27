#ifndef BUILD_LLVM_IR_H
#define BUILD_LLVM_IR_H

#pragma once
#include <vector>
#include "./../../parser.cpp"
#include "./../../synctax_tree.cpp"


//#include "/usr/include/llvm-12/llvm/ADT/APFloat.h"
//#include "/usr/include/llvm-12/llvm/ADT/STLExtras.h"
//#include "/usr/include/llvm-12/llvm/IR/BasicBlock.h"
//#include "/usr/include/llvm-12/llvm/IR/Constants.h"
//#include "/usr/include/llvm-12/llvm/IR/DerivedTypes.h"
//#include "/usr/include/llvm-12/llvm/IR/Function.h"
//#include "/usr/include/llvm-12/llvm/IR/LLVMContext.h"
//#include "/usr/include/llvm-12/llvm/IR/Module.h"
//#include "/usr/include/llvm-12/llvm/IR/Type.h"
//#include "/usr/include/llvm-12/llvm/IR/Verifier.h"
//#include "/usr/include/llvm-12/llvm/IR/IRBuilder.h"

#include "llvm/IR/Module.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
//#include "llvm/"
enum type_of_def_or_dec {
    is_func_decl,
    is_arrary,
    is_var
};

class build_llvm_ir {
    private:
        std::unique_ptr<llvm::LLVMContext> context;
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder;
      //llvm::LLVMContext context;
      //llvm::module("fef", context);
      //llvm::IRBuilder<> builder;

        parser parse;

        std::vector<ast::node> tree;
        type_of_def_or_dec which_type(ast::idx);
        bool is_func_decl(ast::idx);
        bool is_func(ast::idx);
        bool is_struct(ast::idx);
        bool is_union(ast::idx);
        bool is_var(ast::idx);


        llvm::Type *build_mult_declaration_or_defination(ast::idx idx);
        llvm::Type *build_declaration_or_defination(ast::idx idx, llvm::Type *ptr_declaration_declarator);
        llvm::Type *build_declaration_declarator(ast::idx idx);
        llvm::BasicBlock *build_compound_statement();
        void build_variable(ast::idx idx, llvm::Type *ptr_declaration_declarator);
        void build_function_or_function_ptr(ast::idx idx, llvm::Type *ptr_declaration_declarator);
        void build_array(ast::idx idx, llvm::Type *ptr_declaration_declarator);
        void build_funcion_declaration(ast::idx);
        void build_expression_ir();
        void build_assign_expression_ir();

        llvm::SmallVector<llvm::Type*> build_arguments_type_list(ast::idx idx);
        llvm::Type *build_argument_declaration(ast::idx idx);

    public:

        build_llvm_ir();
        ~build_llvm_ir();
        void output_llvm_ir();
};


#endif