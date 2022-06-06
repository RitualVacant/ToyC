#ifndef SYNCTAX_TREE_CPP
#define SYNCTAX_TREE_CPP
#pragma once
#include "./header/class/synctax_tree.h"
#include <fmt/core.h>
#include "./error/inner.cpp"
#include "./header/AST/declataror.h"

synctax_tree::synctax_tree() {
    //if (reserve_tree) tree.reserve(sizeof_synctax_tree_init);
    tree.push_back({});
}

/*
ast::ptr
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
synctax_tree::operator [] (ast::ptr ptr) {
    return tree[ptr];
    /*
    switch (tree[ptr].type) {
        case ast::node_type::type:
        case ast::node_type::expression:
        case ast::node_type::assignment_expression:
        case ast::node_type::conditional_expression:
        case ast::node_type::binary_expression:
        case ast::node_type::unary_expression:
        case ast::node_type::postfix_expression:
        case ast::node_type::primary_expression:
        case ast::node_type::statement:
        case ast::node_type::declare_varible:
        case ast::node_type::declare_function:
        case ast::node_type::definition_function:
        case ast::node_type::constant:
        case ast::node_type::operators:
        case ast::node_type::declarator:
            return tree[ptr].value.declataor;
        default: {

        }
    }
    */
}


//TODO
ast::ptr synctax_tree::creat_node(ast::node_type node_type) {
    tree.push_back({});
    tree.back().type = node_type;
    tree.back().loc = tree.size() - 1;
    return tree.size() - 1;
}


void synctax_tree::connect(ast::ptr ptr) {
    if (last_root_ptr != ast::null) {
        tree[last_root_ptr].next = ptr;
    }
    else {
        last_root_ptr = ptr;
    }
    return;
}

void synctax_tree::print_tree_terminal() {
    file.open(output_file);
    if (!file.is_open()) {
        fmt::print("can't open file in synctax_tree");
        exit(0);
    }
    dfs_tree_print_terminal(last_root_ptr);
    file.close();
    return;
}

void synctax_tree::dfs_tree_print_terminal(ast::ptr ptr) {
    if (ptr == ast::null) return;
    auto &node = tree[ptr].value;
    switch (tree[ptr].type) {
        case ast::node_type::type:
            break;
        case ast::node_type::expression:
            break;
        case ast::node_type::assignment_expression:
            break;
        case ast::node_type::conditional_expression:
            break;
        case ast::node_type::binary_expression:
            break;
        case ast::node_type::unary_expression:
            break;
        case ast::node_type::postfix_expression:
            break;
        case ast::node_type::primary_expression:
            break;
        case ast::node_type::statement:
            break;
        case ast::node_type::declare_varible:
            break;
        case ast::node_type::declare_function:
            std::cout << "\"declare_function\":{\n";
            dfs_tree_print_terminal(tree[ptr].value.declare_funcion.ptr_funcion_arguments);
            dfs_tree_print_terminal(tree[ptr].value.declare_funcion.ptr_return_value_type);
            std::cout << "}\n";
            break;
        case ast::node_type::definition_function:
            std::cout << "\"definition_function\":{\n";
            dfs_tree_print_terminal(tree[ptr].value.definition_function.ptr_funcion_arguments);
            dfs_tree_print_terminal(tree[ptr].value.definition_function.ptr_return_value_type);
            dfs_tree_print_terminal(tree[ptr].value.definition_function.ptr_compound_statement);
            std::cout << "}\n";
            break;
        case ast::node_type::initial_declarator_list:
            std::cout << "\"initial_declarator_list\":{\n";
            dfs_tree_print_terminal(tree[ptr].value.initial_declarator_list.ptr_initial_declarator);
            std::cout << "}\n";
            break;

        case ast::node_type::definition_struct:
            break;
        case ast::node_type::constant:
            break;
        case ast::node_type::operators:
            break;
        case ast::node_type::initial_declarator:
            std::cout << "\"initial_declatator\":{\n";
            dfs_tree_print_terminal(tree[ptr].value.initial_declarator.ptr_declarator);
            dfs_tree_print_terminal(tree[ptr].value.initial_declarator.ptr_initial_value);
            std::cout << "}\n";
            break;

        case ast::node_type::direct_declarator:
            break;

        case ast::node_type::declarator:
            std::cout << "\"declarator\":{\n";
            if (tree[ptr].value.declarator.is_ptr) {
                std::cout << "is_ptr : true\n";
            }
            else {
                std::cout << "is_ptr : false\n";
            }
            dfs_tree_print_terminal(tree[ptr].value.declarator.ptr_direct_declarator);
            std::cout << "}\n";
            break;

        case ast::node_type::declaration_or_definition:
            std::cout << "\"declaration_or_definition\":{\n";
            dfs_tree_print_terminal(node.declaration_or_definition.ptr_declaration_declarator);
            dfs_tree_print_terminal(node.declaration_or_definition.ptr_initial_declatator_list);
            std::cout << "}\n";
            break;

        case ast::node_type::declaration_declarator: {
            std::cout << "\"declaration_declarator\":{\n";
            auto &decl = tree[ptr].value.declaration_declarator;
            if (decl.sign == ast::declarator_sign::sign_signed) {
                std::cout << "\"sign\":true\n";
            }
            else {
                std::cout << "\"sign\":false\n";
            }

            std::cout << "\"type\":";
            switch (decl.type) {
                case ast::declarator_type::type_char:
                    std::cout << "\"char\"";
                    break;
                case ast::declarator_type::type_double:
                    std::cout << "\"double\"";
                    break;
                case ast::declarator_type::type_float:
                    std::cout << "\"float\"";
                    break;
                case ast::declarator_type::type_int:
                    std::cout << "\"int\"";
                    break;
                case ast::declarator_type::type_long_int:
                    std::cout << "\"long_int\"";
                    break;
                case ast::declarator_type::type_long_long_int:
                    std::cout << "\"long_long_int\"";
                    break;
                case ast::declarator_type::type_short_int:
                    std::cout << "\"short_int\"";
                    break;
                case ast::declarator_type::type_void:
                    std::cout << "\"void\"";
                    break;
                default:
                    break;
            }

            std::cout << "\n\"limit\":";
            if (decl.limit == ast::declarator_limit::limit_Alignas) {
                std::cout << "\"Alignas\"\n";
            }
            else {
                std::cout << "\"empty\"\n";
            }

            std::cout << "\"store\":";
            switch (decl.store) {
                case ast::declarator_store::store_empty:
                    std::cout << "\"empty\"";
                    break;
                case ast::declarator_store::store_extern:
                    std::cout << "\"extern\"";
                    break;
                case ast::declarator_store::store_static:
                    std::cout << "\"static\"";
                    break;
                case ast::declarator_store::store_typedef:
                    std::cout << "\"typedef\"";
                    break;
                default:
                    break;
            }
            std::cout << "\n}\n";
            break;
        }
        default: {
            fmt::print("{} {} {} {}", inner::switch_error, __FILE__, __FUNCTION__, __LINE__);
            exit(0);
        }
    }
    return;
}

#endif