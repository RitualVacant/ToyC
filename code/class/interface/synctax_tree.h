#ifndef SYNCTAX_TREE_H
#define SYNCTAX_TREE_H
#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "declataror.h"
#include "ast.h"



std::size_t const sizeof_synctax_tree_init = 100000;



class synctax_tree {
    private:
        bool reserve_tree = false;
        bool print_trans_tree = false;

        enum type_decl_defi {
            func_decl,
            func_defi,
            struct_decl,
            struct_defi,
            array_decl,
            var_decl,
            enum_defi,
        };

        ast::idx last_root_ptr = ast::null;
        std::fstream file;
        std::string file_path{"/home/lzj/code/program/script/test/tree.json"};
        std::string file_buffer;
        std::vector<ast::node> tree;
        void dfs_print_tree(ast::idx ptr);
        void print_json_key_value(char const *key, char const *value);
        void print_json_key(char const *key);
        void print_json_value(char const *value);
        void print_json_class_head(char const *value);
        void print_json_class_end();

        ast::idx idx_now_declaration_declarator = ast::null;
        ast::idx idx_now_compound_statement = ast::null;

        void trans_declaration_or_definination(ast::idx idx);
        void trans_each_initial_declarator(ast::idx idx);
        type_decl_defi which_type(ast::idx idx);

        void trans_to_function(ast::idx idx);
        void trans_to_arrary_definition(ast::idx idx);
        void trans_to_struct_definition(ast::idx idx);
        void trans_to_struct_declaration(ast::idx idx);
        void trans_to_basic_type_declaration(ast::idx idx);
        void trans_to_enum_definition(ast::idx idx);

    public:
        synctax_tree();
        ~synctax_tree();

        ast::idx insert(ast::node_type node_type);
        ast::idx creat_node(ast::node_type node_type);
        void connect(ast::idx ptr);
        void earse(ast::idx idx);
        ast::node& operator [] (ast::idx ptr);
        void print_tree(std::string file_path);
        void to_json();
        void trans_tree();
        std::vector<ast::node> &get_synctax_tree();
};

#endif