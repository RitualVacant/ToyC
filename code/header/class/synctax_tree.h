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
        std::fstream file;
        std::string output_file{"/home/lzj/code/program/script/test/o1"};
        void dfs_tree_print_terminal(ast::ptr ptr);

    public:
        std::vector<ast::node> tree;
        synctax_tree();
        ~synctax_tree() = default;

        ast::ptr insert(ast::node_type node_type);
        ast::ptr creat_node(ast::node_type node_type);
        void connect(ast::ptr ptr);
        void earse(ast::ptr idx);
        ast::node& operator [] (ast::ptr ptr);
        void print_tree_terminal();
        void to_json();
};

#endif