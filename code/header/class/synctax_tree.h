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

        ast::ptr last_root_ptr = ast::null;
        bool to_file = true;
        std::fstream file;
        std::string file_path{"/home/lzj/code/program/script/test/tree.json"};
        void dfs_print_tree(ast::ptr ptr);
        void print_json_key_value(char const *key, char const *value);
        void print_json_key(char const *key);
        void print_json_value(char const *value);
        void print_json_class_head(char const *value);
        void print_json_class_end();

    public:
        std::vector<ast::node> tree;
        synctax_tree();
        ~synctax_tree();

        ast::ptr insert(ast::node_type node_type);
        ast::ptr creat_node(ast::node_type node_type);
        void connect(ast::ptr ptr);
        void earse(ast::ptr idx);
        ast::node& operator [] (ast::ptr ptr);
        void print_tree();
        void to_json();
};

#endif