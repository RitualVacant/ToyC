#ifndef SYNCTAX_TREE_H
#define SYNCTAX_TREE_H
#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "./../AST/declataror.h"
#include "./../AST/ast.h"



std::size_t const sizeof_synctax_tree_init = 100000;



class synctax_tree {
    private:
        bool reserve_tree = false;

        ast::idx last_root_ptr = ast::null;
        std::fstream file;
        std::string file_path{"/home/lzj/code/program/script/test/tree.json"};
        std::string file_buffer;
        void dfs_print_tree(ast::idx ptr);
        void print_json_key_value(char const *key, char const *value);
        void print_json_key(char const *key);
        void print_json_value(char const *value);
        void print_json_class_head(char const *value);
        void print_json_class_end();

    public:
        std::vector<ast::node> tree;
        synctax_tree();
        ~synctax_tree();

        ast::idx insert(ast::node_type node_type);
        ast::idx creat_node(ast::node_type node_type);
        void connect(ast::idx ptr);
        void earse(ast::idx idx);
        ast::node& operator [] (ast::idx ptr);
        void print_tree();
        void to_json();
};

#endif