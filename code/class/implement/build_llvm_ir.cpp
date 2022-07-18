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

    //context->setOpaquePointers(false);
    build_mult_declaration_or_defination(1);
}

build_llvm_ir::~build_llvm_ir() {
}


llvm::Type *
build_llvm_ir::build_mult_declaration_or_defination(ast::idx idx_declaration_or_definiation) {
    for (
        ast::idx i = idx_declaration_or_definiation,
        idx_declaration_declarator = tree[i].value.declaration_or_definition.idx_declaration_declarator;

        i != ast::null;

        i = tree[i].value.declaration_or_definition.idx_next_declaration_or_definition,
        idx_declaration_declarator = tree[i].value.declaration_or_definition.idx_declaration_declarator
    )
    {
        now_compound_statement = tree[i].value.declaration_or_definition.idx_compound_statement;

        for (
            ast::idx j = tree[i].value.declaration_or_definition.idx_initial_declarator;
            j != ast::null;
            j = tree[j].value.initial_declarator.idx_next_initial_declarator
        )
        {
            build_declaration_or_definiation(idx_declaration_declarator, j);
        }
    }
    return nullptr;
}

llvm::Type *
build_llvm_ir::build_declaration_declarator(ast::idx idx) {
    switch (tree[idx].value.declaration_declarator.type) {
        case ast::declarator_type::type_struct:
            return llvm::StructType::get(*context);
        case ast::declarator_type::type_char:
            return llvm::Type::getInt8Ty(*context);
        case ast::declarator_type::type_double:
            return llvm::Type::getDoubleTy(*context);
        case ast::declarator_type::type_float:
            return llvm::Type::getFloatTy(*context);
        case ast::declarator_type::type_short_int:
            return llvm::Type::getInt16Ty(*context);
        case ast::declarator_type::type_int:
        case ast::declarator_type::type_long_int:
            return llvm::Type::getInt32Ty(*context);
        case ast::declarator_type::type_long_long_int:
            return llvm::Type::getInt64Ty(*context);
        //TODO void type
        case ast::declarator_type::type_void:
        default:
            switch_error
    }
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

    //llvm::SmallVector<llvm::Type*> func_args = build_arguments_type_list(idx);
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
    llvm::raw_fd_ostream file("/home/lzj/code/cpp/script/test/llvm.ll", ec);
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
    not_excutable
}

bool
build_llvm_ir::is_union(ast::idx idx) {
    not_excutable
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

void
build_llvm_ir::build_declaration_or_definiation(ast::idx idx_declaration_declarator, ast::idx idx_initial_declarator) {
    ast::idx idx_declarator = tree[idx_initial_declarator].value.initial_declarator.idx_declarator;
    ast::idx idx_direct_declarator = tree[idx_declarator].value.declarator.idx_direct_declarator;

    llvm::Type *ptr_type_declaration_declarator = build_declaration_declarator(idx_declaration_declarator);

    //return type, var type, arrary unit type
    llvm::Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

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
    std::string name{tree[idx_identifier].value.identifier.name};

    //array or a pointer to array
    if (tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null) {
        llvm::Type *ptr_unit_type = ptr_type;
        ast::idx idx_array_declarator
        = tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
        llvm::Type *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

        //a pointer to array
        while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null) {
            idx_declarator = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
            build_pointer(ptr_array_type, idx_declarator);
            idx_direct_declarator
            = tree[idx_declarator].value.declarator.idx_direct_declarator;
        }
        builder->CreateAlloca(ptr_array_type);
        return;
    }
    //todo
    //function
    else if (tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null) {
        llvm::Type *ptr_return_type = ptr_type;
        //get function type list
        llvm::SmallVector<llvm::Type*> argument_type_list = build_arguments_type_list(
            tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
        );



        llvm::FunctionType *ptr_func_type = llvm::FunctionType::get(ptr_return_type, argument_type_list, false);
        //1. a pointer to function
        if (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null) {
            while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null) {
                idx_declarator = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
                build_pointer(ptr_func_type, idx_declarator);
                idx_direct_declarator
                = tree[idx_declarator].value.declarator.idx_direct_declarator;
            }
        }
        //2. a function
        else {
        //3. a funcion declaration
            llvm::Function *ptr_func = llvm::Function::Create(ptr_func_type, llvm::Function::ExternalLinkage, name, *module);

        //4. a funcion definition
            if (now_compound_statement != ast::null) {
                llvm::BasicBlock *ptr_block = llvm::BasicBlock::Create(*context, get_lable(), ptr_func);
                builder->SetInsertPoint(ptr_block);
                ptr_now_func = ptr_func;
                ptr_now_block = ptr_block;

                build_compound_statement(now_compound_statement, ptr_block);
            }
        }

    }
    //struct enum
    else if (now_compound_statement != ast::null) {
        todo
    }
    //var
    else {
        llvm::Value *ptr_var = builder->CreateAlloca(ptr_type);
        //TODO
        //which map
        insert_func_symbol(name, ptr_var, ptr_type);
    }

    return;

    not_excutable
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

        llvm::FunctionType *ptr_func_type = llvm::FunctionType::get(ptr_return_type, argument_type_list, false);
        return ptr_func_type;
    }
    //var
    else {
        return ptr_type;
    }

    not_excutable
}

//TODO
std::size_t
build_llvm_ir::get_constant(ast::idx idx_constant) {
    return 4;
    return tree[idx_constant].value.constant.get_unsigned_int();
}

llvm::BasicBlock *
build_llvm_ir::build_compound_statement(ast::idx idx_compound_statement, llvm::BasicBlock *ptr_block) {
    ast::idx idx_block = tree[idx_compound_statement].value.compound_statement.idx_block;

    for (
        ast::idx i = idx_block;
        i != ast::null;
        i = tree[i].value.block.idx_next_block
    )
    {
        build_block(i);
    }
}

void
build_llvm_ir::build_block(ast::idx idx_block) {
    ast::idx idx_delcaration
    = tree[idx_block].value.block.idx_declaration;
    ast::idx idx_statement
    = tree[idx_block].value.block.idx_statement;
    if (idx_delcaration != ast::null) {
        build_mult_declaration_or_defination(idx_delcaration);
    }
    else {
        switch(tree[idx_statement].type) {
            //TODO all type
            case ast::node_type::if_statement:
                build_if_statement(idx_statement);
                break;
            case ast::node_type::switch_statement:
                build_switch_statement(idx_statement);
                break;
            case ast::node_type::return_statement:
                build_return_statement(idx_statement);
                break;
            case ast::node_type::while_statement:
                build_while_statement(idx_statement);
                break;
            case ast::node_type::do_while_statement:
                build_switch_statement(idx_statement);
                break;
            case ast::node_type::for_statement:
                build_for_statement(idx_statement);
                break;
            case ast::node_type::expression:
                build_expression(idx_statement);
                break;
            default:
                switch_error
        }
    }
    return;
}

void
build_llvm_ir::build_if_statement(ast::idx idx_if_statement) {
  //builder->SetInsertPoint(ptr_block);
    ast::idx idx_assign_expression
    = tree[idx_if_statement].value.if_statement.idx_assign_expression;
    ast::idx idx_else_body
    = tree[idx_if_statement].value.if_statement.idx_else_body;
    ast::idx idx_if_body
    = tree[idx_if_statement].value.if_statement.idx_if_body;

    llvm::BasicBlock *ptr_block_if_body = llvm::BasicBlock::Create(*context, get_lable(), ptr_now_func);

    if (idx_else_body != ast::null) {
        llvm::BasicBlock *ptr_block_else_body = llvm::BasicBlock::Create(*context, get_lable(), ptr_now_func);
        llvm::BasicBlock *ptr_statement_end = llvm::BasicBlock::Create(*context, get_lable(), ptr_now_func);

        build_assign_expression(
            idx_assign_expression,
            ptr_block_if_body,
            ptr_block_else_body
        );
        //if body
        builder->SetInsertPoint(ptr_block_if_body);
        build_compound_statement(idx_if_body);
        builder->CreateBr(ptr_statement_end);
        //else body
        builder->SetInsertPoint(ptr_block_else_body);
        build_compound_statement(idx_else_body);
        //end of if statement
        builder->SetInsertPoint(ptr_statement_end);
    }
    else {
        llvm::BasicBlock *ptr_block_statement_end = llvm::BasicBlock::Create(*context, get_lable(), ptr_now_func);
        build_assign_expression(
            idx_assign_expression,
            ptr_block_if_body,
            ptr_block_statement_end
        );
        //if body
        builder->SetInsertPoint(ptr_block_if_body);
        build_compound_statement(idx_if_body);
        builder->CreateBr(ptr_block_statement_end);
        //end of if statement
        builder->SetInsertPoint(ptr_block_statement_end );
    }
    return;
}

void
build_llvm_ir::build_while_statement(ast::idx idx_while_statement) {

todo
}

void
build_llvm_ir::build_switch_statement(ast::idx idx_switch_statement) {
todo
}


void
build_llvm_ir::build_do_while_statement(ast::idx idx_do_while_statement) {
todo
}

void
build_llvm_ir::build_for_statement(ast::idx idx_for_statement) {
todo
}

void
build_llvm_ir::build_return_statement(ast::idx idx_return_statement) {
    builder->CreateRet(
        build_assign_expression(
            tree[idx_return_statement].value.return_statement.idx_assignment_expression
        )
    );
}

void
build_llvm_ir::build_function(ast::idx idx_function_declaration) {
}

void
build_llvm_ir::build_expression(ast::idx idx_expression) {
    for (
        ast::idx i = idx_expression;
        i != ast::null;
        i = tree[i].value.expression.idx_next_expression
    )
    {
        build_assign_expression(tree[i].value.expression.idx_assignment_expression);
    }
    return;
}

llvm::Value *
build_llvm_ir::build_assign_expression(ast::idx idx_assign_expression ,llvm::BasicBlock *ptr_true_block, llvm::BasicBlock *ptr_false_block) {
    //assign expression
    if (tree[idx_assign_expression].type == ast::node_type::binary_expression) {
        return build_binary_expression(idx_assign_expression, ptr_true_block, ptr_false_block);
    }

    ast::idx idx_unary_or_binary_expression
    = tree[idx_assign_expression].value.assignment_expression.idx_unary_or_binary_expression;

    ast::idx idx_binary_expression
    = tree[idx_assign_expression].value.assignment_expression.idx_binary_expression;

    //just a binary expression without assign
    llvm::Value *r_value = build_binary_expression(idx_binary_expression);
    variable l_value;

    for (
        ast::idx i = idx_assign_expression;
        i != ast::null;
        i = tree[i].value.assignment_expression.idx_next_assignment_expression
    )
    {
        l_value = build_unary_expression(
            tree[i].value.assignment_expression.idx_unary_or_binary_expression
        );
        switch(tree[i].value.assignment_expression.assignment_type) {
            case token::assign:     //=
                builder->CreateStore(r_value, l_value.ptr_value_ptr);
                break;

            case token::plus_agn:   //+=
                builder->CreateStore(
                    builder->CreateAdd(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::minus_agn:  //-=
                builder->CreateStore(
                    builder->CreateSub(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::times_agn:  //*=
                builder->CreateStore(
                    builder->CreateMul(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::div_agn:    ///=
                builder->CreateStore(
                    builder->CreateSDiv(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            //TODO
            case token::mod_agn:    //%=
                break;

            case token::r_shift_agn://>>=
                builder->CreateStore(
                    builder->CreateSDiv(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::l_shift_agn://<<=
                builder->CreateStore(
                    builder->CreateSDiv(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::bit_and_agn://&=
                builder->CreateStore(
                    builder->CreateSDiv(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::bit_or_agn: //|=
                builder->CreateStore(
                    builder->CreateSDiv(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            case token::bit_xor_agn://^=
                builder->CreateStore(
                    builder->CreateSDiv(
                        r_value,
                        builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr)
                    ),
                    l_value.ptr_value_ptr
                );
                break;

            default:
                switch_error
        }


        //if (tree[i].value.assignment_expression.idx_next_assignment_expression != ast::null) {
        r_value = builder->CreateLoad(l_value.ptr_type, l_value.ptr_value_ptr);
        //}
    }

    //jump
    if (ptr_true_block != nullptr) {
        builder->CreateCondBr(
            builder->CreateICmpNE(
                r_value,
                builder->getInt1(false)
            ),
            ptr_true_block,
            ptr_false_block
        );
    }
    return r_value;
}

//TODO
build_llvm_ir::variable
build_llvm_ir::build_unary_expression(ast::idx idx_unary_expression) {
    //
    if (tree[idx_unary_expression].value.unary_expression.unary_operator != token::invalid) {

    }
    //
    else if (tree[idx_unary_expression].value.unary_expression.is_sizeof) {

    }
    //
    else if (tree[idx_unary_expression].value.unary_expression.idx_postfix_expression) {

    }
    //convertion
    else if (tree[idx_unary_expression].value.unary_expression.idx_declaration_declatator) {

    }

    ast::idx idx_postifx_expression
    = tree[idx_unary_expression].value.unary_expression.idx_postfix_expression;

    ast::idx idx_primary_expression
    = tree[idx_postifx_expression].value.postfix_expression.idx_primary_expression;

    ast::idx idx_postfix_operator
    = tree[idx_postifx_expression].value.postfix_expression.idx_postfix_operator;

    //TODO
    variable value_loc_and_type = find_value(
        std::string{
            tree[idx_primary_expression].value.identifier.name
        }
    );
    return value_loc_and_type;
    //if (return_location_ptr) return value_loc_and_type.value;
    //return builder->CreateLoad(value_loc_and_type.type, value_loc_and_type.value);
}


build_llvm_ir::variable
build_llvm_ir::find_value(std::string name) {
    if (func_symbol_table.find(name) != func_symbol_table.end()) {
        return func_symbol_table.at(name);
    }
    if (global_symbol_table.find(name) != global_symbol_table.end()) {
        return global_symbol_table.at(name);
    }
    not_excutable
}

llvm::Value *
build_llvm_ir::build_primary_expression(ast::idx idx_primary_expression) {
    not_excutable
}

llvm::Value *
build_llvm_ir::build_binary_expression(ast::idx idx_binary_expression, llvm::BasicBlock *ptr_true_block, llvm::BasicBlock *ptr_false_block) {
    if (tree[idx_binary_expression].type == ast::node_type::unary_expression) {
        auto var_type_value = build_unary_expression(idx_binary_expression);
        return builder->CreateLoad(
            var_type_value.ptr_type,
            var_type_value.ptr_value_ptr
        );
    }

  //llvm::Value *value_l
  //= build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
  //llvm::Value *value_r
  //= build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
    llvm::Value *value_l;
    llvm::Value *value_r;

    switch (tree[idx_binary_expression].value.binary_expression.token_operator) {
        //TODO
        //float num
        case token::log_and: {

            //
            //left value
            //
            value_l = build_binary_expression(
                tree[idx_binary_expression].value.binary_expression.idx_left_node,
                ptr_true_block,
                ptr_false_block
            );

            ptr_true_block = llvm::BasicBlock::Create(*context, get_lable(), ptr_now_func);

            auto ret_val = builder->CreateICmpNE(
                value_l,
                llvm::ConstantInt::get(value_l->getType(), 0)
            );

            builder->CreateCondBr(
                ret_val,
                ptr_true_block,
                ptr_false_block
            );
            builder->SetInsertPoint(ptr_true_block);

            //
            //right value
            //
            value_r = build_binary_expression(
                tree[idx_binary_expression].value.binary_expression.idx_right_node,
                ptr_true_block,
                ptr_false_block
            );


            return ret_val;
        }

        case token::log_or:

            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);

            builder->CreateCondBr(
                builder->CreateICmpNE(
                    value_l,
                    builder->getInt1(false)
                ),
                ptr_true_block,
                ptr_false_block
            );
            builder->SetInsertPoint(ptr_true_block);

            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);

            return builder->CreateOr(value_l, value_r);

        case token::bit_and:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
        todo

        case token::bit_or:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
        todo

        case token::bit_xor:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
        todo

        case token::equ:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateICmpEQ(value_l, value_r);

        case token::not_equ:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateICmpNE(value_l, value_r);

        case token::great:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateICmpSGT(value_l, value_r);

        case token::great_equ:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateICmpSGE(value_l, value_r);

        case token::less:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateICmpSLT(value_l, value_r);

        case token::less_equ:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateICmpSLE(value_l, value_r);

        case token::l_shift:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);

        //todo
        case token::r_shift:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);

        //todo
        case token::plus:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateAdd(value_l, value_r);

        case token::times:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            return builder->CreateFSub(value_l, value_r);

        case token::minus:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            todo

        case token::div:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            todo

        case token::mod:
            value_l
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
            value_r
            = build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
            todo

        default:
            switch_error
    }
    not_excutable
}


void
build_llvm_ir::insert_func_symbol(std::string name, llvm::Value *ptr_var, llvm::Type *ptr_type) {
    if (
        func_symbol_table.find(name) != func_symbol_table.end()
        || global_symbol_table.find(name) != global_symbol_table.end()
    )
    {
        fmt::print("redeclaration of {}", name);
        exit(0);
    }
    func_symbol_table.insert({name, variable{ptr_var, ptr_type}});
    return;
}

void
build_llvm_ir::insert_symbol_symbol(std::string name, llvm::Value *ptr_var, llvm::Type *ptr_type) {
    if (
        func_symbol_table.find(name) != func_symbol_table.end()
        || global_symbol_table.find(name) != global_symbol_table.end()
    )
    {
        fmt::print("redeclaration of {}", name);
        exit(0);
    }
    func_symbol_table.insert({name, variable{ptr_var, ptr_type}});
    return;
}

std::string
build_llvm_ir::get_lable() {
    return std::to_string(++lable_num);
}
#endif
