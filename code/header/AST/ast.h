#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "token.h"
#endif

namespace ast {
    //basic
    struct node {
        std::size_t line = 0;
        std::size_t column = 0;

        explicit node(std::size_t line,std::size_t colume) : line{line}, column{column} {};
        virtual ~node() = default;
    };
    //表达式
    struct expression : public node {

        explicit expression(std::size_t line,std::size_t colume) : node(line, column) {}; //委托构造
    };
    //语句
    struct statement : public node {

        explicit statement(std::size_t line,std::size_t colume) : node(line, column) {}; //委托构造
    };

    //expression node

    //bool
    struct bool_expr : public expression {
        bool value;

        explicit bool_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };

    //int
    struct int_expr : public expression {
        int value;

        explicit int_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };
    //char
    struct char_expr : public expression {
        char value;

        explicit char_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };
    //null
    struct null_expr : public expression {

        explicit null_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };
    //double
    struct double_expr : public expression {
        double value;

        explicit double_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };
    //string
    struct string_expr : public expression {
        std::string value;

        explicit string_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };
    //array
    struct array_expr : public expression {
        std::vector<expression*> value;

        explicit array_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };

    //indentfi
    //include var name and func name
    struct indentfi_expr : public expression {
        std::string indentfi_name;
        explicit indentfi_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };

    //index []
    struct index_expr : public expression {
        std::string index_name;
        expression* index;
        explicit index_expr(std::size_t line, std::size_t colume) : expression(line, column) {};

    };

    //二元表达式
    struct binary_expr : public expression {
        expression* l_expr = nullptr;
        expression* r_expr = nullptr;
        token expr_operator = token::invalid;
        explicit binary_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };

    struct func_call_expr : public expression {
        //函数名称
        std::string func_name;
        //函数参数
        std::vector<expression*> args;

        explicit func_call_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };

    struct assign_expr : public expression {
        expression* l_expr = nullptr;
        expression* r_expr = nullptr;
        token expr_operator = token::invalid;
        explicit assign_expr(std::size_t line, std::size_t colume) : expression(line, column) {};
    };

    // statement node

    //break
    struct break_statement : public statement {

        explicit break_statement(std::size_t line,std::size_t colume) : statement(line, column) {}; //委托构造
    };
    //continue
    struct continue_statement : public statement {

        explicit continue_statement(std::size_t line,std::size_t colume) : statement(line, column) {}; //委托构造
    };
    //expression
    struct expr_statement : public statement {

        explicit expr_statement(std::size_t line,std::size_t colume) : statement(line, column) {}; //委托构造
    };
    //return
    struct return_statement : public statement {
        ast::expression* return_expression = nullptr;
        explicit return_statement(std::size_t line,std::size_t colume) : statement(line, column) {}; //委托构造
    };
    //if
    struct if_statement : public statement {
        ast::expression* jugement_statement = nullptr;
        std::vector<ast::statement*> if_block;
        std::vector<ast::statement*> else_block;

        explicit if_statement(std::size_t line,std::size_t colume) : statement(line, column) {}; //委托构造
    };
    //while
    struct while_statement : public statement {
        ast::expression* jugement_statement = nullptr;
        std::vector<ast::statement*> while_block;

        explicit while_statement(std::size_t line,std::size_t colume) : statement(line, column) {}; //委托构造
    };
}