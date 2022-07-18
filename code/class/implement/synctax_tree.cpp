#ifndef SYNCTAX_TREE_CPP
#define SYNCTAX_TREE_CPP
#pragma once
#include "synctax_tree.h"
#include <fmt/format.h>
#include <fmt/core.h>
#include "inner.cpp"
#include "declataror.h"
#include <stdlib.h>

synctax_tree::synctax_tree() {
    if (reserve_tree) {
        tree.reserve(sizeof_synctax_tree_init);
    }
    tree.push_back({});
}

synctax_tree::~synctax_tree() {
    //int done = system("python3.8 -m json.tool /home/lzj/code/program/script/test/tree.json /home/lzj/code/program/script/test/tree1.json");
    //if (done == 127) {
    //    fmt::print("output json tree");
    //}
    //if (done == -1) {
    //    fmt::print("output json tree");
    //}
}

/*
ast::idx
synctax_tree::insert(ast::node_type node_type) {
    //type_list.push_back(node_type);
    tree.push_back({});
    switch (node_type) {
        case ast::node_type::type: {
        }
        case ast::node_type::expression: {
        }
        case ast::node_type::assignment_expression: {

        }
        case ast::node_type::conditional_expression:
        case ast::node_type::binary_expression:
        case ast::node_type::unary_expression:
        case ast::node_type::postfix_expression:
        case ast::node_type::primary_expression:
        case ast::node_type::statement:
        case ast::node_type::declare_function: {
            //tree.back().value.declare_funcion.loc.line = line;
            //tree.back().value.declare_funcion.loc.column = column;

        }
        case ast::node_type::declare_varible: {

        }
        case ast::node_type::definition_function: {

        }
        case ast::node_type::constant:
        case ast::node_type::operators:
        default: {

        }
    }
    //++size;
    //++end;
    //return end;
}
*/

ast::node&
synctax_tree::operator [] (ast::idx idx) {
    return tree[idx];
}


//TODO
ast::idx synctax_tree::creat_node(ast::node_type node_type) {
    tree.push_back({});
    tree.back().type = node_type;
    /*
    switch (node_type) {
        case ast::node_type::type:
            tree.back().value.type.is_init = true;
            break;
        case ast::node_type::expression:
            tree.back().value.expression.is_init = true
            break;
        case ast::node_type::assignment_expression:
            tree.back().
            break;
        case ast::node_type::conditional_expression:
            tree.back().
            break;
        case ast::node_type::binary_expression:
            tree.back().
            break;
        case ast::node_type::unary_expression:
            tree.back().
            break;
        case ast::node_type::postfix_expression:
            tree.back().
            break;
        case ast::node_type::primary_expression:
            tree.back().
            break;
        case ast::node_type::statement:
            tree.back().
            break;
        case ast::node_type::declare_varible:
            tree.back().
            break;
        case ast::node_type::declare_function:
            tree.back().
            break;
        case ast::node_type::definition_function:
            tree.back().
            break;
        case ast::node_type::definition_struct:
            tree.back().
            break;
        case ast::node_type::constant:
            tree.back().
            break;
        case ast::node_type::operators:
            tree.back().
            break;
        case ast::node_type::initial_declarator:
            tree.back().
            break;
        case ast::node_type::direct_declarator:
            tree.back().
            break;
        case ast::node_type::declarator:
            tree.back().
            break;
        case ast::node_type::declaration_declarator:
            tree.back().
            break;
        case ast::node_type::initial_declarator_list:
            tree.back().
            break;
        case ast::node_type::declaration_or_definition:
            tree.back().
            break;
        case ast::node_type::identifier:
            tree.back().
            break;
        case ast::node_type::arguments_declaration:
            tree.back().
            break;
        case ast::node_type::arguments_type_list:
            tree.back().
            break;
        case ast::node_type::compound_statement:
            tree.back().
            break;
        case ast::node_type::block_list:
            tree.back().
            break;
        case ast::node_type::block:
            tree.back().
            break;
        case ast::node_type::mark_statement:
            tree.back().
            break;
        default:
            switch_error
    }
    */
    return tree.size() - 1;
}

std::vector<ast::node> &synctax_tree::get_synctax_tree() {
    //DROP
    //trans_tree();
    return tree;
}

//
void
synctax_tree::trans_tree() {
    print_trans_tree = true;
    for (
        ast::idx i = 1;
        i != ast::null;
        i = tree[i].value.declaration_or_definition.idx_next_declaration_or_definition
    )
    {
        trans_declaration_or_definination(i);
    }
    return;
}

void
synctax_tree::trans_declaration_or_definination(ast::idx idx_declaration_or_definition) {
    ast::idx idx_start_initial_declarator
    = tree[idx_declaration_or_definition].value.declaration_or_definition.idx_initial_declarator;

    idx_now_declaration_declarator
    = tree[idx_declaration_or_definition].value.declaration_or_definition.idx_declaration_declarator;

    idx_now_compound_statement
    = tree[idx_declaration_or_definition].value.declaration_or_definition.idx_compound_statement;

    //trans will cover the next idx value in declarator
    //j store next idx before next idx been covered
    for (
        ast::idx i = idx_start_initial_declarator,
                 j = tree[i].value.initial_declarator.idx_next_initial_declarator;
        i != ast::null;
        i = j
    )
    {
        //std::cout << i << ' ' << j << std::endl;
        trans_each_initial_declarator(i);
    }
    return;
}

void
synctax_tree::trans_each_initial_declarator(ast::idx idx_initial_declarator) {
    switch(which_type(idx_initial_declarator)) {
        case type_decl_defi::array_decl:
            tree[idx_initial_declarator].type = ast::node_type::array_definintion;
            trans_to_arrary_definition(idx_initial_declarator);
            break;
        case type_decl_defi::enum_defi:
            tree[idx_initial_declarator].type = ast::node_type::enum_definition;
            trans_to_enum_definition(idx_initial_declarator);
            break;
        case type_decl_defi::func_decl:
        case type_decl_defi::func_defi:
            trans_to_function(idx_initial_declarator);
            break;
        case type_decl_defi::struct_decl:
            trans_to_struct_declaration(idx_initial_declarator);
            break;
        case type_decl_defi::struct_defi:
            trans_to_struct_definition(idx_initial_declarator);
            break;
        case type_decl_defi::var_decl:
            tree[idx_initial_declarator].type = ast::node_type::basic_type_declaration;
            trans_to_basic_type_declaration(idx_initial_declarator);
            break;
        default:
            switch_error
    }
    return;
}

synctax_tree::type_decl_defi
synctax_tree::which_type(ast::idx idx_initial_declarator) {

    ast::idx idx_direct_declarator =
        tree[
            tree[
                idx_initial_declarator
            ].value.initial_declarator.idx_declarator
        ].value.declarator.idx_direct_declarator;

    //function
    if (tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null) {
        if (tree[idx_initial_declarator].value.declaration_or_definition.idx_compound_statement != ast::null) {
            return type_decl_defi::func_defi;
        }
        else {
            return type_decl_defi::func_decl;
        }
    }

    //array
    if (tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null) {
        return type_decl_defi::array_decl;
    }

    //struct
    if (tree[idx_now_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_struct) {
        if (idx_now_compound_statement != ast::null) {
            return type_decl_defi::struct_defi;
        }
        else {
            return type_decl_defi::struct_decl;
        }
    }

    //enum
    if (tree[idx_now_declaration_declarator].value.declaration_declarator.type == ast::declarator_type::type_enum) {
        return type_decl_defi::enum_defi;
    }

    return var_decl;
}

void
synctax_tree::trans_to_function(ast::idx idx) {
    tree[idx].type = ast::node_type::function;

    ast::idx idx_next_declarator = tree[idx].value.initial_declarator.idx_next_initial_declarator;
    ast::idx idx_declarator = tree[idx].value.initial_declarator.idx_declarator;
    ast::idx idx_identifier;

    //because the function pointer
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

    ast::idx idx_arguments_type_list =
        tree[
            tree [
                tree[
                    idx
                ].value.initial_declarator.idx_declarator
            ].value.declarator.idx_direct_declarator
        ].value.direct_declarator.idx_arguments_type_list;

    //
    //cover
    //

    //function name
    tree[idx].value.function_declaration.idx_function_name
    = idx_identifier;

    //function type and return type
    tree[idx].value.function_declaration.idx_function_declarator
    = idx_declarator;

    //return type
    tree[idx].value.function_declaration.idx_function_return_type
    = idx_now_declaration_declarator;

    //function body
    tree[idx].value.function_declaration.idx_function_body
    = idx_now_compound_statement;

    //next declaration
    tree[idx].value.function_declaration.idx_next
    = idx_next_declarator;

    //arguments list
    tree[idx].value.function_declaration.idx_function_arguments_type_list
    = idx_arguments_type_list;
    return;
}

void
synctax_tree::trans_to_arrary_definition(ast::idx idx_initial_declarator) {
    tree[idx_initial_declarator].type = ast::node_type::struct_declaration;

    //type of struct
    tree[idx_initial_declarator].value.struct_defination.idx_struct_type
    = idx_now_declaration_declarator;

    //struct idnentifer
    tree[idx_initial_declarator].value.struct_defination.idx_struct_name
    =   tree[
            tree[
                tree[idx_initial_declarator].value.initial_declarator.idx_declarator
            ].value.declarator.idx_direct_declarator
        ].value.direct_declarator.idx_identifier;

    return;
}
void
synctax_tree::trans_to_struct_definition(ast::idx idx_initial_declarator) {
    tree[idx_initial_declarator].type = ast::node_type::struct_defination;

    //type of struct
    tree[idx_initial_declarator].value.struct_defination.idx_struct_type
    = idx_now_declaration_declarator;

    //struct body
    tree[idx_initial_declarator].value.struct_defination.idx_struct_body
    = idx_now_compound_statement;

    //struct idnentifer
    tree[idx_initial_declarator].value.struct_defination.idx_struct_name
    =   tree[
            tree[
                tree[idx_initial_declarator].value.initial_declarator.idx_declarator
            ].value.declarator.idx_direct_declarator
        ].value.direct_declarator.idx_identifier;

    return;
}
void
synctax_tree::trans_to_struct_declaration(ast::idx idx_initial_declarator) {
    tree[idx_initial_declarator].type = ast::node_type::struct_declaration;
    return;
}
void
synctax_tree::trans_to_basic_type_declaration(ast::idx idx) {
    return;
}
void
synctax_tree::trans_to_enum_definition(ast::idx idx) {
    return;
}

void
synctax_tree::connect(ast::idx idx) {
    if (last_root_ptr != ast::null) {
        tree[last_root_ptr].value.declaration_or_definition.idx_next_declaration_or_definition
        = idx;
    }
    else {
        last_root_ptr = idx;
    }
    return;
}

void
synctax_tree::print_tree(std::string file_path) {
    file.open(file_path, std::fstream::out);
    if (!file.is_open()) {
        fmt::print("can't open file in synctax_tree");
        exit(0);
    }

    file_buffer.clear();
    file_buffer += '{';
    dfs_print_tree(last_root_ptr);
    file_buffer += '}';

    //earse extra comma in json file string
    //just repalce comma with /n
    for (std::size_t i = 0, j = 1; j < file_buffer.size(); ++i, ++j) {
        if (file_buffer[i] == ',' && file_buffer[j] == '}') {
            file_buffer[i] = '\n';
        }
    }

    file << file_buffer;
    file.close();

    return;
}

void
synctax_tree::dfs_print_tree(ast::idx idx) {
    if (idx == ast::null) return;
    now_idx = idx;
    switch (tree[idx].type) {
        case ast::node_type::assignment_expression:
            print_json_class_head("assignment_expression");
            print_json_key("assignment_type");
            switch (tree[idx].value.assignment_expression.assignment_type) {
                case token::assign:
                    print_json_value("=");
                    break;
                case token::plus_agn:
                    print_json_value("+=");
                    break;
                case token::minus_agn:
                    print_json_value("-=");
                    break;
                case token::times_agn:
                    print_json_value("*=");
                    break;
                case token::div_agn:
                    print_json_value("/=");
                    break;
                case token::mod_agn:
                    print_json_value("%=");
                    break;
                case token::r_shift_agn:
                    print_json_value(">>=");
                    break;
                case token::l_shift_agn:
                    print_json_value("<<=");
                    break;
                case token::bit_and_agn:
                    print_json_value("&=");
                    break;
                case token::bit_or_agn:
                    print_json_value("|=");
                    break;
                case token::bit_xor_agn:
                    print_json_value("^=");
                    break;
                default:
                    switch_error
            }
            dfs_print_tree(tree[idx].value.assignment_expression.idx_binary_expression);
            dfs_print_tree(tree[idx].value.assignment_expression.idx_unary_or_binary_expression);
            print_json_class_head("next_assgin");
            dfs_print_tree(tree[idx].value.assignment_expression.idx_next_assignment_expression);
            print_json_class_end();
            print_json_class_end();
            break;

        case ast::node_type::unary_expression:
            print_json_class_head("unary_expression");
            dfs_print_tree(tree[idx].value.unary_expression.idx_declaration_declatator);
            dfs_print_tree(tree[idx].value.unary_expression.idx_postfix_expression);
            dfs_print_tree(tree[idx].value.unary_expression.idx_unary_expression);

            print_json_key("is_sizeof");
            if (tree[idx].value.unary_expression.is_sizeof) {
                print_json_value("true");
            }
            else {
                print_json_value("false");
            }

            print_json_key("unary_operator");
            switch (tree[idx].value.unary_expression.unary_operator) {
                case token::bit_and:
                    print_json_value("&");
                    break;
                case token::log_not:
                    print_json_value("!");
                    break;
                case token::bit_not:
                    print_json_value("~");
                    break;
                case token::times:
                    print_json_value("*");
                    break;
                case token::invalid:
                    print_json_value("void");
                    break;
                default:
                    std::cout << fmt::format(fg(fmt::color::red), "the operator is {}\n", tree[idx].value.unary_expression.unary_operator);
                    switch_error
                break;
            }
            print_json_class_end();
            break;

        case ast::node_type::while_statement:
            print_json_class_head("while_statement");
            dfs_print_tree(tree[idx].value.while_statement.idx_assignment_expression);
            dfs_print_tree(tree[idx].value.while_statement.idx_compound_statement);
            print_json_class_end();
            break;

        case ast::node_type::postfix_expression:
            print_json_class_head("postfix_expression");
            dfs_print_tree(tree[idx].value.postfix_expression.idx_postfix_operator);
            dfs_print_tree(tree[idx].value.postfix_expression.idx_primary_expression);
            print_json_class_end();
            break;

        case ast::node_type::binary_expression:
            print_json_class_head("binary_expression");
            print_json_key("operator");
            switch (tree[idx].value.binary_expression.token_operator) {
                case token::log_or:
                    print_json_value("||");
                    break;
                case token::log_and:
                    print_json_value("&&");
                    break;
                case token::bit_or:
                    print_json_value("|");
                    break;
                case token::bit_xor:
                    print_json_value("^");
                    break;
                case token::bit_and:
                    print_json_value("&");
                    break;
                case token::equ:
                    print_json_value("==");
                    break;
                case token::not_equ:
                    print_json_value("!=");
                    break;
                case token::less:
                    print_json_value("<");
                    break;
                case token::less_equ:
                    print_json_value("<=");
                    break;
                case token::great:
                    print_json_value(">");
                    break;
                case token::great_equ:
                    print_json_value(">=");
                    break;
                case token::l_shift:
                    print_json_value("<<");
                    break;
                case token::r_shift:
                    print_json_value(">>");
                    break;
                case token::plus:
                    print_json_value("+");
                    break;
                case token::minus:
                    print_json_value("-");
                    break;
                case token::div:
                    print_json_value("/");
                    break;
                case token::mod:
                    print_json_value("%");
                    break;
                case token::times:
                    print_json_value("*");
                    break;
                default:
                    switch_error
            }
            dfs_print_tree(tree[idx].value.binary_expression.idx_left_node);
            dfs_print_tree(tree[idx].value.binary_expression.idx_right_node);
            print_json_class_end();
            break;

        case ast::node_type::expression:
            print_json_class_head("expression");
            dfs_print_tree(tree[idx].value.expression.idx_assignment_expression);
            print_json_class_end();
            dfs_print_tree(tree[idx].value.expression.idx_next_expression);
            break;

        /*
        case ast::node_type::type:
            print_json_class_head("type");
            print_json_class_end();
            break;

        case ast::node_type::conditional_expression:
            print_json_class_head("conditional_expression");
            print_json_class_end();
            break;

        case ast::node_type::primary_expression:
            print_json_class_head("primary_expression");
            print_json_class_end();
            break;
        case ast::node_type::statement:
            print_json_class_head("statement");
            print_json_class_end();
            break;
        case ast::node_type::declare_varible:
            print_json_class_head("declare_varible");
            print_json_class_end();
            break;
        */
        case ast::node_type::initializer:
            print_json_class_head("initializer");
            dfs_print_tree(tree[idx].value.initializer.idx_assignment_expression);
            dfs_print_tree(tree[idx].value.initializer.idx_initializer_list);
            print_json_class_end();
            break;
        case ast::node_type::declare_function:
            print_json_class_head("declare_function");
            dfs_print_tree(tree[idx].value.declare_funcion.idx_funcion_arguments);
            dfs_print_tree(tree[idx].value.declare_funcion.idx_return_value_type);
            print_json_class_end();
            break;
        case ast::node_type::definition_function:
            print_json_class_head("definition_function");
            dfs_print_tree(tree[idx].value.definition_function.idx_funcion_arguments);
            dfs_print_tree(tree[idx].value.definition_function.idx_return_value_type);
            dfs_print_tree(tree[idx].value.definition_function.idx_compound_statement);
            print_json_class_end();
            break;
        case ast::node_type::initial_declarator_list:
            print_json_class_head("initial_declarator_list");
            dfs_print_tree(tree[idx].value.initial_declarator_list.idx_initial_declarator);
            print_json_class_end();
            break;

        case ast::node_type::definition_struct:
            print_json_class_head("definition_struct");
            print_json_class_end();
            break;

        case ast::node_type::constant:
            print_json_class_head("constant");
            print_json_key_value("value", tree[idx].value.constant.const_value);
            print_json_class_end();
            break;

        case ast::node_type::operators:
            print_json_class_head("operators");
            print_json_class_end();
            break;

        case ast::node_type::initial_declarator:
            print_json_class_head("initial_declarator");
            dfs_print_tree(tree[idx].value.initial_declarator.idx_declarator);
            dfs_print_tree(tree[idx].value.initial_declarator.idx_initializer);
            dfs_print_tree(tree[idx].value.initial_declarator.idx_next_initial_declarator);
            print_json_class_end();
            break;

        case ast::node_type::direct_declarator:
            print_json_class_head("direct_declarator");
            dfs_print_tree(tree[idx].value.direct_declarator.idx_identifier);
            dfs_print_tree(tree[idx].value.direct_declarator.idx_declarator);
            dfs_print_tree(tree[idx].value.direct_declarator.idx_array_declarator);
            if (!print_trans_tree) {
                dfs_print_tree(tree[idx].value.direct_declarator.idx_arguments_type_list);
            }
            print_json_class_end();
            break;

        case ast::node_type::array_declarator:
            print_json_class_head("array_declarator");
            dfs_print_tree(tree[idx].value.array_declarator.idx_constant);
            dfs_print_tree(tree[idx].value.array_declarator.idx_next_array_declarator);
            print_json_class_end();
            break;

        case ast::node_type::declarator:
            print_json_class_head("declarator");
            switch (tree[idx].value.declarator.is_ptr) {
                case 0:
                    print_json_key_value("is_pointer", "0");
                    break;
                case 1:
                    print_json_key_value("is_pointer", "1");
                    break;
                case 2:
                    print_json_key_value("is_pointer", "2");
                    break;
                case 3:
                    print_json_key_value("is_pointer", "3");
                    break;
                case 4:
                    print_json_key_value("is_pointer", "4");
                    break;
                case 5:
                    print_json_key_value("is_pointer", "5");
                    break;
                default:
                    fmt::print("pointer level can't over 5");
                    switch_error
            }
            dfs_print_tree(tree[idx].value.declarator.idx_direct_declarator);
            print_json_class_end();
            break;

        case ast::node_type::declaration_or_definition:
            print_json_class_head("declaration_or_definition");
            if (!print_trans_tree) {
                dfs_print_tree(tree[idx].value.declaration_or_definition.idx_declaration_declarator);
                dfs_print_tree(tree[idx].value.declaration_or_definition.idx_compound_statement);
            }
            dfs_print_tree(tree[idx].value.declaration_or_definition.idx_initial_declarator);
            dfs_print_tree(tree[idx].value.declaration_or_definition.idx_next_declaration_or_definition);
            print_json_class_end();
            break;

        case ast::node_type::identifier:
            print_json_class_head("indentifier");
            print_json_key_value("name", tree[idx].value.identifier.name);
            print_json_class_end();
            break;

        case ast::node_type::arguments_type_list:
            print_json_class_head("arguments_type_list");
            dfs_print_tree(tree[idx].value.arguments_type_list.idx_argument_declaration);
            print_json_class_end();
            break;

        case ast::node_type::arguments_declaration:
            print_json_class_head("arguments_declaration");
            dfs_print_tree(tree[idx].value.arguments_declaration.idx_declararion_declarator);
            dfs_print_tree(tree[idx].value.arguments_declaration.idx_declarator);
            print_json_class_end();
            dfs_print_tree(tree[idx].value.arguments_declaration.idx_next_arguments_declatation);
            break;

        case ast::node_type::compound_statement:
            print_json_class_head("compound_statement");
            dfs_print_tree(tree[idx].value.compound_statement.idx_block);
            print_json_class_end();
            break;

        case ast::node_type::block:
            print_json_class_head("block");
            dfs_print_tree(tree[idx].value.block.idx_declaration);
            dfs_print_tree(tree[idx].value.block.idx_statement);
            print_json_class_end();
            dfs_print_tree(tree[idx].value.block.idx_next_block);
            break;

        case ast::node_type::postfix_operator:
            print_json_class_head("postfix_operator");
            dfs_print_tree(tree[idx].value.postfix_operator.idx_array_idx_assignment_expression);
            dfs_print_tree(tree[idx].value.postfix_operator.idx_identifier);
            dfs_print_tree(tree[idx].value.postfix_operator.idx_func_call_assignment_expression_list);
            dfs_print_tree(tree[idx].value.postfix_operator.idx_next_postfix_operator);
            print_json_class_end();
            break;

        case ast::node_type::if_statement:
            print_json_class_head("if_statement");
            dfs_print_tree(tree[idx].value.if_statement.idx_assign_expression);
            dfs_print_tree(tree[idx].value.if_statement.idx_if_body);
            dfs_print_tree(tree[idx].value.if_statement.idx_else_body);
            print_json_class_end();
            break;

        case ast::node_type::for_statement:
            print_json_class_head("for_statement");
            dfs_print_tree(tree[idx].value.for_statement.idx_declaration);
            dfs_print_tree(tree[idx].value.for_statement.idx_conditional_assign_expression);
            dfs_print_tree(tree[idx].value.for_statement.idx_change_assign_expression);
            dfs_print_tree(tree[idx].value.for_statement.idx_compound_statement);
            print_json_class_end();
            break;

        case ast::node_type::switch_statement:
            print_json_class_head("switch_statement");
            dfs_print_tree(tree[idx].value.switch_statement.idx_assign_expression);
            dfs_print_tree(tree[idx].value.switch_statement.idx_compound_statement);
            print_json_class_end();
            break;

        case ast::node_type::do_while_statement:
            print_json_class_head("do_while_statement");
            dfs_print_tree(tree[idx].value.do_while_statement.idx_assign_statement);
            dfs_print_tree(tree[idx].value.do_while_statement.idx_compound_statement);
            print_json_class_end();
            break;

        case ast::node_type::continue_statement:
            print_json_class_head("continue_statement");
            print_json_class_end();
            break;

        case ast::node_type::break_statement:
            print_json_class_head("break_statement");
            print_json_class_end();
            break;
        case ast::node_type::default_label:
            print_json_class_head("default_label");
            print_json_class_end();
            break;

        case ast::node_type::case_label:
            print_json_class_head("case_lable");
            dfs_print_tree(tree[idx].value.case_label.const_expression);
            print_json_class_end();
            break;

        //TODO
        case ast::node_type::basic_type_declaration:
            print_json_class_head("basic_type_declaration");
            dfs_print_tree(tree[idx].value.basic_type_declaration.idx_next);
            print_json_class_end();
            break;

        case ast::node_type::struct_declaration:
            print_json_class_head("struct_declaration");
            dfs_print_tree(tree[idx].value.struct_defination.idx_struct_type);
            dfs_print_tree(tree[idx].value.struct_defination.idx_struct_name);
            print_json_class_end();
            break;

        case ast::node_type::struct_defination:
            print_json_class_head("struct_defination");
            dfs_print_tree(tree[idx].value.struct_defination.idx_struct_type);
            dfs_print_tree(tree[idx].value.struct_defination.idx_struct_name);
            dfs_print_tree(tree[idx].value.struct_defination.idx_struct_body);
            print_json_class_end();
            break;

        case ast::node_type::declaration_declarator: {
            print_json_class_head("declaration_declarator");
            auto &decl = tree[idx].value.declaration_declarator;
            if (decl.sign == ast::declarator_sign::sign_signed) {
                print_json_key_value("sign", "true");
            }
            else {
                print_json_key_value("sign", "false");
            }

            print_json_key("type");
            switch (decl.type) {
                case ast::declarator_type::type_char:
                    print_json_value("char");
                    break;
                case ast::declarator_type::type_double:
                    print_json_value("double");
                    break;
                case ast::declarator_type::type_float:
                    print_json_value("float");
                    break;
                case ast::declarator_type::type_int:
                    print_json_value("int");
                    break;
                case ast::declarator_type::type_long_int:
                    print_json_value("long_int");
                    break;
                case ast::declarator_type::type_long_long_int:
                    print_json_value("long_long_int");
                    break;
                case ast::declarator_type::type_short_int:
                    print_json_value("short_int");
                    break;
                case ast::declarator_type::type_void:
                    print_json_value("void");
                    break;
                case ast::declarator_type::type_struct:
                    print_json_value("struct");
                    break;
                default:
                    switch_error
            }

            print_json_key("limit");
            if (decl.limit == ast::declarator_limit::limit_Alignas) {
                print_json_value("Alignas");
            }
            else {
                print_json_value("empty");
            }

            print_json_key("store");
            switch (decl.store) {
                case ast::declarator_store::store_empty:
                    print_json_value("empty");
                    break;
                case ast::declarator_store::store_extern:
                    print_json_value("extern");
                    break;
                case ast::declarator_store::store_static:
                    print_json_value("static");
                    break;
                case ast::declarator_store::store_typedef:
                    print_json_value("typedef");
                    break;
                default:
                    break;
            }
            print_json_class_end();
            break;
        }


        //second step node
        case ast::node_type::function:
            print_json_class_head("function_declaration");
            dfs_print_tree(tree[idx].value.function_declaration.idx_function_name);
            dfs_print_tree(tree[idx].value.function_declaration.idx_function_declarator);
            dfs_print_tree(tree[idx].value.function_declaration.idx_function_return_type);
            dfs_print_tree(tree[idx].value.function_declaration.idx_function_arguments_type_list);
            dfs_print_tree(tree[idx].value.function_declaration.idx_function_body);
            dfs_print_tree(tree[idx].value.function_declaration.idx_next);
            print_json_class_end();
            break;

        case ast::node_type::return_statement:
            print_json_class_head("return_statement");
            dfs_print_tree(tree[idx].value.return_statement.idx_assignment_expression);
            print_json_class_end();
            break;

        default: {
            fmt::print(fg(fmt::color::red), "add node type : {}\n", tree[idx].type);
            switch_error
        }
    }
    return;
}

void
synctax_tree::print_json_key_value(char const *key, char const *value) {
    //file << "\"" << key << "\":\"" << value << "\",\n";
    auto str_key = std::string(key);
    auto str_value = std::string(value);
    file_buffer += fmt::format("\"{}\":\"{}\",", str_key, str_value);
  //file_buffer += "\"";
  //file_buffer += std::string(key);
  //file_buffer += "\":\"";
  //file_buffer += std::string(value);
  //file_buffer += "\",";
    return;
}

void
synctax_tree::print_json_class_head(char const *value) {
    //file << "\"" <<  value << "\":{\n";

    //auto str_value = std::string(value);
    //file_buffer += fmt::format("\"{}\":{", str_value);
    file_buffer += "\"";
    file_buffer += std::string(value);
    file_buffer += '_';
    file_buffer += std::to_string(now_idx);
    file_buffer += "\":{";
    return;
}

void
synctax_tree::print_json_class_end() {
    //file << "},\n";
    //file_buffer += fmt::format("},");
    file_buffer += "},";
    return;
}

void
synctax_tree::print_json_key(char const *key) {
    //file << "\"" << key << "\":";
    //auto str_key = std::string(key);
    //file_buffer += fmt::format("\"{}\":", str_key);
    file_buffer += "\"";
    file_buffer += std::string(key);
    file_buffer += "\":";
    return;
}

void
synctax_tree::print_json_value(char const *value) {
    //
    //file << "\"" << value << "\",\n";
    //auto str_value = std::string(value);
    //file_buffer += fmt::format("\"{}\",", str_value);
    file_buffer += "\"";
    file_buffer += std::string(value);
    file_buffer += "\",";
    return;
}

#endif