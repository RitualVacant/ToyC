#ifndef SYNCTAX_TREE_CPP
#define SYNCTAX_TREE_CPP
#pragma once
#include "./header/class/synctax_tree.h"
#include <fmt/core.h>
#include "./error/inner.cpp"
#include "./header/AST/declataror.h"
#include <stdlib.h>

synctax_tree::synctax_tree() {
    //if (reserve_tree) tree.reserve(sizeof_synctax_tree_init);
    if (to_file) {
        file.open(file_path, std::fstream::out);
        if (!file.is_open()) {
            fmt::print("can't open file in synctax_tree");
            exit(0);
        }
    }
    tree.push_back({});
}

synctax_tree::~synctax_tree() {
    file.close();
    int done = system("pytm-cli -i ~/code/program/script/test/tree.json -t 3");
    if (done == 127) {
        fmt::print("output json tree");
    }
    if (done == -1) {
        fmt::print("output json tree");
    }
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
synctax_tree::operator [] (ast::idx ptr) {
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


void synctax_tree::connect(ast::idx ptr) {
    if (last_root_ptr != ast::null) {
        tree[last_root_ptr].next = ptr;
    }
    else {
        last_root_ptr = ptr;
    }
    return;
}

void synctax_tree::print_tree() {

    if (to_file) file << "{\n";
    else std::cout << "{\n";

    dfs_print_tree(last_root_ptr);

    if (to_file) file << "}\n";
    else std::cout << "}\n";
    file.close();
    return;
}

void synctax_tree::dfs_print_tree(ast::idx ptr) {
    if (ptr == ast::null) return;
    switch (tree[ptr].type) {
        case ast::node_type::type:
            print_json_class_head("type");
            print_json_class_end();
            break;
        case ast::node_type::expression:
            print_json_class_head("expression");
            print_json_class_end();
            break;
        case ast::node_type::assignment_expression:
            print_json_class_head("assignment_expression");
            print_json_class_end();
            break;
        case ast::node_type::conditional_expression:
            print_json_class_head("conditional_expression");
            print_json_class_end();
            break;
        case ast::node_type::binary_expression:
            print_json_class_head("binary_expression");
            print_json_class_end();
            break;
        case ast::node_type::unary_expression:
            print_json_class_head("unary_expression");
            print_json_class_end();
            break;
        case ast::node_type::postfix_expression:
            print_json_class_head("postfix_expression");
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
        case ast::node_type::initializer:
            print_json_class_head("initializer");
            dfs_print_tree(tree[ptr].value.initializer.idx_assignment_expression);
            dfs_print_tree(tree[ptr].value.initializer.idx_initializer_list);
            print_json_class_end();
            break;
        case ast::node_type::declare_function:
            print_json_class_head("declare_function");
            dfs_print_tree(tree[ptr].value.declare_funcion.idx_funcion_arguments);
            dfs_print_tree(tree[ptr].value.declare_funcion.idx_return_value_type);
            print_json_class_end();
            break;
        case ast::node_type::definition_function:
            print_json_class_head("definition_function");
            dfs_print_tree(tree[ptr].value.definition_function.idx_funcion_arguments);
            dfs_print_tree(tree[ptr].value.definition_function.idx_return_value_type);
            dfs_print_tree(tree[ptr].value.definition_function.idx_compound_statement);
            print_json_class_end();
            break;
        case ast::node_type::initial_declarator_list:
            print_json_class_head("initial_declarator_list");
            dfs_print_tree(tree[ptr].value.initial_declarator_list.idx_initial_declarator);
            print_json_class_end();
            break;

        case ast::node_type::definition_struct:
            print_json_class_head("definition_struct");
            print_json_class_end();
            break;
        case ast::node_type::constant:
            print_json_class_head("constant");
            print_json_class_end();
            break;
        case ast::node_type::operators:
            print_json_class_head("operators");
            print_json_class_end();
            break;
        case ast::node_type::initial_declarator:
            print_json_class_head("initial_declarator");
            dfs_print_tree(tree[ptr].value.initial_declarator.idx_declarator);
            dfs_print_tree(tree[ptr].value.initial_declarator.idx_initial_value);
            std::cout << tree[ptr].value.initial_declarator.idx_initial_value << std::endl;
            dfs_print_tree(tree[ptr].value.initial_declarator.idx_next_initial_declarator);
            std::cout << tree[ptr].value.initial_declarator.idx_next_initial_declarator << std::endl;
            print_json_class_end();
            break;

        case ast::node_type::direct_declarator:
            print_json_class_head("direct_declarator");
            dfs_print_tree(tree[ptr].value.direct_declarator.idx_identifier);
            dfs_print_tree(tree[ptr].value.direct_declarator.idx_declarator);
            dfs_print_tree(tree[ptr].value.direct_declarator.idx_arguments_type_list);
            print_json_class_end();
            break;

        case ast::node_type::declarator:
            print_json_class_head("declarator");
            if (tree[ptr].value.declarator.is_ptr) {
                print_json_key_value("is_ptr", "true");
            }
            else {
                print_json_key_value("is_ptr", "false");
            }
            dfs_print_tree(tree[ptr].value.declarator.idx_direct_declarator);
            print_json_class_end();
            break;

        case ast::node_type::declaration_or_definition:
            print_json_class_head("declaration_or_definition");
            dfs_print_tree(tree[ptr].value.declaration_or_definition.idx_declaration_declarator);
            dfs_print_tree(tree[ptr].value.declaration_or_definition.idx_initial_declatator_list);
            print_json_class_end();
            break;

        case ast::node_type::identifier:
            print_json_class_head("indentifier");
            print_json_key_value("name", tree[ptr].value.identifier.name);
            print_json_class_end();
            break;

        case ast::node_type::arguments_type_list:
            print_json_class_head("arguments_type_list");
            dfs_print_tree(tree[ptr].value.arguments_type_list.idx_argument_declaration);
            print_json_class_end();
            break;

        case ast::node_type::arguments_declaration:
            print_json_class_head("arguments_declaration");
            dfs_print_tree(tree[ptr].value.arguments_declaration.idx_declararion_declarator);
            dfs_print_tree(tree[ptr].value.arguments_declaration.idx_declarator);
            dfs_print_tree(tree[ptr].value.arguments_declaration.idx_next_arguments_declatation);
            print_json_class_end();
            break;

        case ast::node_type::declaration_declarator: {
            print_json_class_head("declaration_declarator");
            auto &decl = tree[ptr].value.declaration_declarator;
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
                default:
                    break;
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
        default: {
            switch_error
        }
    }
    return;
}

void
synctax_tree::print_json_key_value(char const *key, char const *value) {
    if (to_file) {
        file << "\"" << key << "\":\"" << value << "\",\n";
    }
    else {
        printf("\"%s\":\"%s\",\n", key, value);
    }

    return;
}

void
synctax_tree::print_json_class_head(char const *value) {
    if (to_file) {
        file << "\"" <<  value << "\":{\n";
    }
    else {
        printf("\"%s\":{\n", value);
    }
    return;
}

void
synctax_tree::print_json_class_end() {
    if (to_file) {
        file << "},\n";
    }
    else {
        printf("},\n");
    }
    return;
}

void
synctax_tree::print_json_key(char const *key) {
    if (to_file) {
        file << "\"" << key << "\":";
    }
    else {
        printf("\"%s\":", key);
    }
    return;
}

void
synctax_tree::print_json_value(char const *value) {
    if (to_file) {
        file << "\"" << value << "\",\n";
    }
    else {
        printf("\"%s\",\n", value);
    }
    return;
}

#endif