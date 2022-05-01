#ifndef SYNCTAX_TREE_CPP
#define SYNCTAX_TREE_CPP
#pragma once
#include "./header/class/synctax_tree.h"
#include <fmt/core.h>


synctax_tree::synctax_tree() {
    //if (reserve_tree) tree.reserve(sizeof_synctax_tree_init);
    tree.push_back({});
}

ast::ptr
synctax_tree::insert(ast::node_type node_type) {
    type_list.push_back(node_type);
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
            tree.back().value.declare_funcion.loc.line = line;
            tree.back().value.declare_funcion.loc.column = column;

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
    ++size;
    ++end;
    return end;
}

ast::node&
synctax_tree::operator[] (ast::ptr ptr) {
    switch (type_list[ptr]) {
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
        default: {
            fmt::print("unknow error at synctax_tree::operator[] (ast::ptr ptr)");
            exit(0);
        }
    }
}
#endif