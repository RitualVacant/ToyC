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

    context->setOpaquePointers(false);
    build_mult_declaration_or_defination();
}

build_llvm_ir::~build_llvm_ir() {
}


llvm::Type *
build_llvm_ir::build_mult_declaration_or_defination() {

    for (
        ast::idx i{1};
        i != ast::null;
        i = tree[i].value.declaration_or_definition.idx_next_declaration_or_definition
    )
    {
        for (
            ast::idx j = tree[i].value.declaration_or_definition.idx_initial_declatator_list;
            j != ast::null;
        )
        {
            switch (tree[j].type) {
                case ast::node_type::function:
                    build_function(j);
                    j = tree[j].value.function_declaration.idx_next;
                    break;
                case ast::node_type::struct_declaration:
                    todo
                    j = tree[j].value.struct_declaration.idx_next;
                    break;
                case ast::node_type::struct_defination:
                    todo
                    j = tree[j].value.struct_defination.idx_next;
                    break;
                case ast::node_type::array_declarator:
                    todo
                    break;
                default:
                    fmt::print(fg(fmt::color::red), "node type is {}\n", tree[j].type);
                    switch_error
            }
        }
    }

    return nullptr;
}

llvm::Type *
build_llvm_ir::build_declaration_declarator(ast::idx idx) {
    switch (tree[idx].value.declaration_declarator.type) {
        case ast::declarator_type::type_char:
            return builder->getInt8Ty();
        case ast::declarator_type::type_double:
            return builder->getDoubleTy();
        case ast::declarator_type::type_float:
            return builder->getFloatTy();
        case ast::declarator_type::type_short_int:
            return builder->getInt16Ty();
        case ast::declarator_type::type_int:
        case ast::declarator_type::type_long_int:
            return builder->getInt32Ty();
        case ast::declarator_type::type_long_long_int:
            return builder->getInt64Ty();
        case ast::declarator_type::type_void:
        default:
            switch_error
    }
}

//DROP
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
            //build_array(idx, ptr_declaration_declarator);
            break;
        default:
            switch_error
    }
    //TODO return a true pointer
    return nullptr;
}

llvm::SmallVector<llvm::Type*>
build_llvm_ir::build_arguments_type_list(ast::idx idx_arguemnt_type_list) {

    node_type_wrong(tree[idx_arguemnt_type_list].type, ast::node_type::arguments_type_list)

    //traverse each argument declatation in the arguments type list
    llvm::SmallVector<llvm::Type*> arguments_type_list;
    for (
        ast::idx i = tree[idx_arguemnt_type_list].value.arguments_type_list.idx_argument_declaration;
        i != ast::null;
        i = tree[i].value.arguments_declaration.idx_next_arguments_declatation
    )
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
build_llvm_ir::build_argument_declaration(ast::idx idx_argument_declaration) {
    ast::idx idx_declaration_declarator
    = tree[idx_argument_declaration].value.arguments_declaration.idx_declararion_declarator;

    ast::idx idx_declarator
    = tree[idx_argument_declaration].value.arguments_declaration.idx_declarator;

    llvm::Type *ptr_type_argument = build_type(idx_declaration_declarator, idx_declarator);

    return ptr_type_argument;
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

llvm::Type *
build_llvm_ir::build_array(llvm::Type *ptr_unit_type, ast::idx idx_array_declarator) {
    ast::idx idx_next_array_declarator
    = tree[idx_array_declarator].value.array_declarator.idx_next_array_declarator;

    ast::idx idx_constant
    = tree[idx_array_declarator].value.array_declarator.idx_constant;
    if (idx_next_array_declarator != ast::null) {
        return llvm::ArrayType::get(
            build_array(
                ptr_unit_type,
                idx_next_array_declarator
            ),
            get_constant(idx_constant)
        );
    }
    else {
        return llvm::ArrayType::get(
            ptr_unit_type,
            get_constant(idx_constant)
        );
    }
}

llvm::Type *
build_llvm_ir::build_recurs_array(llvm::Type *ptr_unit_type, ast::idx idx_array_declarator) {
    ast::idx idx_next_array_declarator
    = tree[idx_array_declarator].value.array_declarator.idx_next_array_declarator;

    ast::idx idx_constant
    = tree[idx_array_declarator].value.array_declarator.idx_constant;
    if (idx_next_array_declarator != ast::null) {
        return llvm::ArrayType::get(
            build_recurs_array(
                ptr_unit_type,
                idx_next_array_declarator
            ),
            get_constant(idx_constant)
        );
    }
    else {
        return llvm::ArrayType::get(
            ptr_unit_type,
            get_constant(idx_constant)
        );
    }
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
    std::error_code ec;
    llvm::raw_fd_ostream file("/home/lzj/code/program/script/test/llvm", ec);
    module->print(file, nullptr);
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

llvm::Type *
build_llvm_ir::build_pointer(llvm::Type *ptr_type_declaration_declarator, ast::idx idx_declarator) {
    for (
        auto i = tree[idx_declarator].value.declarator.is_ptr;
        i > 0;
        --i
    )
    {
        ptr_type_declaration_declarator = llvm::PointerType::get(ptr_type_declaration_declarator, 0);
    }
    return ptr_type_declaration_declarator;
}

llvm::Type *
build_llvm_ir::build_type(ast::idx idx_declaration_declarator, ast::idx idx_declarator) {


    llvm::Type *ptr_type_declaration_declarator = build_declaration_declarator(idx_declaration_declarator);

    //return type, var type, arrary unit type
    llvm::Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

    ast::idx idx_direct_declarator = tree[idx_declarator].value.declarator.idx_direct_declarator;

    //array or a pointer to array
    if (tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null) {
        llvm::Type *ptr_unit_type = ptr_type;
        ast::idx idx_array_declarator
        = tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
        llvm::Type *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

        //a pointer to array
        ast::idx idx_declarator;
        while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null) {
            idx_declarator = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
            build_pointer(ptr_array_type, idx_declarator);
            idx_direct_declarator
            = tree[idx_declarator].value.declarator.idx_direct_declarator;
        }

        return ptr_array_type;
    }
    //todo
    //function
    else if (tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null) {
        llvm::Type *ptr_return_type = ptr_type;
        //get function type list
        llvm::SmallVector<llvm::Type*> argument_type_list = build_arguments_type_list(
            tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
        );

        //get funcion name
        ast::idx idx_identifier;
        for (ast::idx i = idx_declarator; tree[i].type != ast::node_type::identifier;) {
            switch (tree[i].type) {
                case ast::node_type::declarator:
                    i = tree[i].value.declarator.idx_direct_declarator;
                    break;
                case ast::node_type::direct_declarator:
                    if (tree[i].value.direct_declarator.idx_identifier != ast::null) {
                        i = tree[i].value.direct_declarator.idx_identifier;
                        idx_identifier = i;
                    }
                    else {
                        i = tree[i].value.direct_declarator.idx_declarator;
                    }
                    break;
                default:
                    fmt::print(fg(fmt::color::red), "node type {}\n", tree[i].type);
                    switch_error
            }
        }
        char *func_name = tree[idx_identifier].value.identifier.name;

        llvm::FunctionType *ptr_func = llvm::FunctionType::get(ptr_return_type, argument_type_list, false);
        llvm::Function::Create(ptr_func, llvm::Function::ExternalLinkage, func_name, *module);
    }
    //var struct enum
    else {
        return ptr_type;
    }

    not_excutable
}

//TODO
std::size_t
build_llvm_ir::get_constant(ast::idx idx_constant) {

}

void
build_llvm_ir::build_function(ast::idx idx_function_declaration) {
    //get function return type

    llvm::Type *return_type = build_declaration_declarator(ast::idx );
    ast::idx idx_function_declarator
    = tree[idx_function_declaration].value.function_declaration.idx_function_declarator;

    //TODO pointer
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
    llvm::Function::Create(ptr_func, llvm::Function::ExternalLinkage, func_name, *module);

    //create function body
    //build_compound_statement();

    //llvm::BasicBlock *func_body = build_block();
    return;



    //------------------------------------------------------

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
}

llvm::BasicBlock *
build_llvm_ir::build_compound_statement(ast::idx idx_compound_statement, llvm::Function *ptr_func , llvm::BasicBlock *ptr_block) {
    //TODO
    llvm::BasicBlock::Create(*context, "", ptr_func, ptr_block);
}

#endif
