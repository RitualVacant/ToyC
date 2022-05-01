#ifndef SYNCTAX_TREE_H
#define SYNCTAX_TREE_H
#pragma once
#include <vector>
#include <string>

#include "./../AST/declararor.h"
#include "./../AST/ast.h"



std::size_t const sizeof_synctax_tree_init = 100000;



class synctax_tree {
    private:
        bool reserve_tree = false;

        std::size_t size = 0;
        ast::ptr end = 1;
        std::vector<ast::node> tree;
        std::vector<ast::node_type> type_list;

    public:
        synctax_tree();
        ~synctax_tree() = default;

        ast::ptr insert(ast::node_type node_type);
        void earse(ast::ptr idx);
        ast::node& operator[] (ast::ptr ptr);
};

#endif