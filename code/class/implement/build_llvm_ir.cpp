#ifndef BUILD_LLVM_IR_CPP
#define BUILD_LLVM_IR_CPP

#pragma once
#include "build_llvm_ir.h"
#include "parser.cpp"

build_llvm_ir::build_llvm_ir() {
    tree = std::move(parse.get_synctax_tree());
    context = std::make_unique<llvm::LLVMContext>();
    module  = std::make_unique<llvm::Module>("text_module", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);

    build_mult_declaration_or_defination();
}

build_llvm_ir::~build_llvm_ir() {
}


llvm::Type *
build_llvm_ir::build_mult_declaration_or_defination() {
    /*
    ast::idx idx_declaration_declarator = tree[idx].value.declaration_or_definition.idx_declaration_declarator;
    ast::idx idx_initial_declarator = tree[idx].value.declaration_or_definition.idx_initial_declatator_list;
    ast::idx idx_compound_statement = tree[idx].value.declaration_or_definition.idx_compound_statement;
    llvm::Type *ptr_declaration_declarator = build_declaration_declarator(idx_declaration_declarator);

    //is function or struct

    llvm::Type *function;
    for (
        ast::idx i = idx_initial_declarator;
        i != ast::null;
        i = tree[i].value.initial_declarator.idx_next_initial_declarator
    )
    {
        function = build_declaration_or_defination(i, ptr_declaration_declarator);
    }

    if (idx_compound_statement != ast::null) {
        build_compound_statement(idx_compound_statement);
    }
    //TODO return a true pointer
    return nullptr;
    */

    for (
        ast::idx i{1};
        i != ast::null;
        i = tree[i].value.declaration_or_definition.idx_next_declaration_or_definition
    )
    {
        std::cout << "i" << i;
        for (
            ast::idx j = tree[i].value.declaration_or_definition.idx_initial_declatator_list;
            j != ast::null;
        )
        {
            std::cout << "j" << j;
            switch (tree[j].type) {
                case ast::node_type::function_declaration:
                    build_function_declaration(j);
                    j = tree[j].value.function_declaration.idx_next;
                    //goto out;
                    break;
                case ast::node_type::function_definition:
                    todo
                    j = tree[j].value.function_definition.idx_next;
                    //goto out;
                    break;
                case ast::node_type::struct_declaration:
                    todo
                    j = tree[j].value.struct_declaration.idx_next;
                    //goto out;
                    break;
                case ast::node_type::struct_defination:
                    todo
                    j = tree[j].value.struct_defination.idx_next;
                    //goto out;
                    break;
                case ast::node_type::array_declarator:
                    todo
                    //goto out;
                    break;
                default:
                    fmt::print(fg(fmt::color::red), "node type is {}\n", tree[j].type);
                    switch_error
            }
        }
    }

    return nullptr;

    for (
        ast::idx idx_decl_defi{1};
        idx_decl_defi != ast::null;
        idx_decl_defi = tree[idx_decl_defi].value.declaration_or_definition.idx_next_declaration_or_definition
    )
    {
      //if (idx_decl_defi  != ast::null) {
      //    std::cout << std::endl;
      //    std::cout << idx_decl_defi << "!=" << ast::null;
      //    //std::cout << typeid(i).name() << std::endl;
      //    //std::cout << typeid(ast::null).name() << std::endl;
      //    //std::cout << (typeid(i) == typeid(ast::null)) << std::endl;
      //    if (idx_decl_defi == ast::null) {
      //        std::cout << idx_decl_defi << "==" << ast::null << std::endl << std::endl;
      //    }
      //}
        for (
            ast::idx j = tree[idx_decl_defi].value.declaration_or_definition.idx_initial_declatator_list;
            j != ast::null;
        )
        {
            //std::cout << ' ' << j << std::endl;
            switch (tree[j].type) {
                case ast::node_type::function_declaration:
                    build_function_declaration(j);
                    j = tree[j].value.function_declaration.idx_next;
                    //goto out;
                    break;
                case ast::node_type::function_definition:
                    todo
                    j = tree[j].value.function_definition.idx_next;
                    //goto out;
                    break;
                case ast::node_type::struct_declaration:
                    todo
                    j = tree[j].value.struct_declaration.idx_next;
                    //goto out;
                    break;
                case ast::node_type::struct_defination:
                    todo
                    j = tree[j].value.struct_defination.idx_next;
                    //goto out;
                    break;
                case ast::node_type::array_declarator:
                    todo
                    //goto out;
                    break;
                default:
                    fmt::print(fg(fmt::color::red), "node type is {}\n", tree[j].type);
                    switch_error
            }
        }
    }
}

llvm::Type *
build_llvm_ir::build_declaration_declarator(ast::idx idx) {
}

llvm::Type *
build_llvm_ir::build_declaration_or_defination(ast::idx idx, llvm::Type *ptr_declaration_declarator) {
    switch (which_type(idx)) {
        case type_of_def_or_dec::is_var:
            build_variable(idx, ptr_declaration_declarator);
            break;
        case type_of_def_or_dec::is_func_decl:
            build_function_or_function_ptr(idx, ptr_declaration_declarator);
            break;
        case type_of_def_or_dec::is_arrary:
            build_array(idx, ptr_declaration_declarator);
            break;
        default:
            switch_error
    }
    //TODO return a true pointer
    return nullptr;
}

llvm::SmallVector<llvm::Type*>
build_llvm_ir::build_arguments_type_list(ast::idx idx_initial_declarator) {
    ast::idx idx_argu_type_list
    = tree[
        tree[
            tree[
                 idx_initial_declarator
            ].value.initial_declarator.idx_declarator
        ].value.declarator.idx_direct_declarator
    ].value.direct_declarator.idx_arguments_type_list;

    node_type_wrong(tree[idx_argu_type_list].type, ast::node_type::arguments_type_list)

    //traverse each argument declatation in the arguments type list
    llvm::SmallVector<llvm::Type*> arguments_type_list;
    for (ast::idx i = tree[idx_argu_type_list].value.arguments_type_list.idx_argument_declaration;
        tree[i].value.arguments_declaration.idx_next_arguments_declatation != ast::null;
        i = tree[i].value.arguments_declaration.idx_next_arguments_declatation)
    {
        arguments_type_list.push_back(build_argument_declaration(i));
    }
    return arguments_type_list;
}

//TODO
//all type declarator
//1. array's pointer
//2.
llvm::Type *
build_llvm_ir::build_argument_declaration(ast::idx idx) {
    ast::idx idx_declaration_declarator
    = tree[idx].value.arguments_declaration.idx_declararion_declarator;

    ast::idx idx_declarator
    = tree[idx].value.arguments_declaration.idx_declarator;

    llvm::Type *ptr_declarator = nullptr;
    switch (tree[idx_declaration_declarator].value.declaration_declarator.type) {
        case ast::declarator_type::type_char:
            ptr_declarator = builder->getInt8Ty();
            break;
        case ast::declarator_type::type_double:
            ptr_declarator = builder->getDoubleTy();
            break;
        case ast::declarator_type::type_float:
            ptr_declarator = builder->getFloatTy();
            break;
        case ast::declarator_type::type_int:
            ptr_declarator = builder->getInt32Ty();
            break;
        case ast::declarator_type::type_long_int:
            ptr_declarator = builder->getInt32Ty();
            break;
        case ast::declarator_type::type_long_long_int:
            ptr_declarator = builder->getInt64Ty();
            break;
        case ast::declarator_type::type_short_int:
            ptr_declarator = builder->getInt16Ty();
            break;
        case ast::declarator_type::type_void:
            //TODO
            return nullptr;
            break;
        default:
            switch_error
    }

    //TODO
    //pointer
    goto __seg;

    for (auto i = tree[idx_declarator].value.declarator.is_ptr; i > 0; --i) {

    }

    __seg:

    //TODO
    //return type
    return ptr_declarator;
}

//TODO
//1. function pointer
//
void
build_llvm_ir::build_function_or_function_ptr(ast::idx idx, llvm::Type *ptr_declaration_declarator) {

    llvm::SmallVector<llvm::Type*> func_args = build_arguments_type_list(idx);
    //llvm::FunctionType *ptr_func = llvm::FunctionType::get();
    return;
}


void
build_llvm_ir::build_variable(ast::idx idx, llvm::Type *ptr_declaration_declarator) {

}
void
build_llvm_ir::build_array(ast::idx idx, llvm::Type *ptr_declaration_declarator) {

}

bool
build_llvm_ir::is_func_decl(ast::idx idx) {
    switch (tree[idx].type) {
        case ast::node_type::declaration_or_definition:
            return is_func_decl(tree[idx].value.declaration_or_definition.idx_declaration_declarator);
        case ast::node_type::initial_declarator_list:
            return is_func_decl(tree[idx].value.initial_declarator_list.idx_initial_declarator);
        case ast::node_type::initial_declarator:
            return is_func_decl(tree[idx].value.initial_declarator.idx_declarator);
        case ast::node_type::declarator:
            return tree[idx].value.declarator.idx_direct_declarator;
        case ast::node_type::direct_declarator:
            if (tree[idx].value.direct_declarator.idx_declarator != ast::null) {
                return is_func_decl(tree[idx].value.direct_declarator.idx_declarator);
            }
            else {
                if (tree[idx].value.direct_declarator.idx_arguments_type_list != ast::null) {
                    return true;
                }
                else {
                    return false;
                }
            }
        default:
            switch_error
    }
}

void
build_llvm_ir::output_llvm_ir() {
    module->dump();
    return;
}


//function is used on initial declarator node
//cause a declaration include several initail declarator
type_of_def_or_dec
build_llvm_ir::which_type(ast::idx idx) {
    while (true) {
        switch(tree[idx].type) {
            case ast::node_type::initial_declarator:
                idx = tree[idx].value.initial_declarator.idx_declarator;
                break;
            case ast::node_type::declarator:
                idx = tree[idx].value.declarator.idx_direct_declarator;
                break;
            case ast::node_type::direct_declarator:
                if (tree[idx].value.direct_declarator.idx_arguments_type_list != ast::null) {
                    return type_of_def_or_dec::is_func_decl;
                }
                else if (tree[idx].value.direct_declarator.idx_array_declarator != ast::null) {
                    return type_of_def_or_dec::is_arrary;
                }
                else {
                    return type_of_def_or_dec::is_var;
                }
            default:
                switch_error
        }
    }
    not_excutable
}

bool
build_llvm_ir::is_struct(ast::idx idx) {

}

bool
build_llvm_ir::is_union(ast::idx idx) {

}



void
build_llvm_ir::build_function_declaration(ast::idx idx_function_declaration) {
    std::cout << "build_function_declaration" << std::endl;
    //fmt::print("build_function_declaration\n");
    //get function return type

    /*
    llvm::Type *return_type = build_declaration_declarator(
        tree[idx_function_declaration].value.function_declaration.idx_function_return_type
    );

    ast::idx idx_function_declarator
    = tree[idx_function_declaration].value.function_declaration.idx_function_declarator;

    for (
        auto i = tree[idx_function_declarator].value.declarator.is_ptr;
        i > 0;
        --i
    )
    {
        return_type = llvm::PointerType::get(return_type, 0);
        //return_type = builder->getPtrTy(return_type, 0);
    }

    //get function type list
    auto argument_type_list = build_arguments_type_list(
        tree[idx_function_declaration].value.function_declaration.idx_function_arguments_type_list
    );

    //get function name
    char const *func_name
    = tree[
        tree[idx_function_declaration].value.function_declaration.idx_function_name
    ].value.identifier.name;

    //create funcion
    llvm::FunctionType *ptr_func = llvm::FunctionType::get(return_type, argument_type_list, false);
    llvm::Function *ptr_func_ = llvm::Function::Create(ptr_func, llvm::Function::ExternalLinkage, func_name, *module);
    */


    /*
    llvm::LLVMContext Context;
    llvm::Module *mod = new llvm::Module("sum.ll", Context);

    //1、创建IRBuilder
    llvm::IRBuilder<> builder(Context);
    //2、创建main函数
    llvm::FunctionType *ft = llvm::FunctionType::get(builder.getInt32Ty(),false);
    llvm::Function *mainfunc = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", mod);
    //到此为止之创建了main函数，但是函数体内的包含的Instruction没有添加，因此需要添加。

    //3、创建基本块（这个基本块是空的无内容）
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context,"entrypoint",mainfunc);

    //4、设置插入点:插入点设置成相应BasicBlock，<#后面用builder创建的指令都会追加到这个BasicBlock里了#>
    //!!!: - 理解：上面的方式是通过直接往BasicBloock中添加Instruction方式来构造基本的basicBlock，这里借助IRBuilder方式，往basicBlock中添加命令。
    builder.SetInsertPoint(entry);

    //5、添加全局字符串（IR中字符串全部为全局变量，使用数据序列来表示，每个元素是一个char类型）
    llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!\n");
    //6、创建put函数
    //1)指定函数参数类型，装在一个数组中`
    std::vector<llvm::Type*> putsargs;
    putsargs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsargs);
    //2）指定函数返回值类型
    llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(),argsRef,false);
    //3)创建“函数调用”，而不是创建函数
    llvm::FunctionCallee putsFunc = mod->getOrInsertFunction("puts", putsType);

    //7、调用函数（<#理解：通过createXXX创建出来的所有指令都在SetInsertPoint后面#>）
    builder.CreateCall(putsFunc,helloWorld); //这是创建方法的指令

    //8、创建返回ret指令
    llvm::ConstantInt *zero = llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(Context), 0);
    builder.CreateRet(zero);

    //9、验证。这一步待定！
    llvm::VerifierAnalysis::Result Res;
    Res.IRBroken = llvm::verifyModule(*mod, &llvm::dbgs(), &Res.DebugInfoBroken);

    mod->dump();
    //mod->print();

    return;
    */
}

#endif
