#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef TUPLE_H
#define TUPLE_H
#include <tuple>
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "./../AST/token.h"
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef AST_H
#define AST_H
#include "./../AST/ast.h"
#endif

#ifndef SCANNING_CPP
#define SCANNING_CPP
#include "./../../scanning.cpp"
#endif

class parser {
    private:
        unsigned int line = 0;
        unsigned int column = 0;
        std::tuple<token, std::string> &&now_token = {};
        std::size_t num_token = 0;  //the number of token in this script file
        std::string file_path;

        //词法分析
        scanning* scan;
        //词法分析器

        //创造节点
        ast::if_statement*        parser_if_statement ();
        ast::while_statement*     parser_while_statement();
        ast::return_statement*    parser_return_statement();

        //解析----没有完工
        ast::expression* parser_expression();
        ast::expression* parser_unary_expression();
        ast::expression* parser_primary_expression();
        std::vector<ast::statement*> parse_block();

        //输出语法分析中的错误信息且终止进程
        void output_error_message(token);

    public:
        //parser(std::string file_path_) : file_path(file_path_) {};
        explicit parser(std::string &file_path_) : file_path(file_path_) {
            scan = new scanning(file_path);
        };
        ~parser() {
            delete scan;
        };

        void create_node() {
            while (!scan->file.eof()) {
                auto &&a = scan->next_token();
                switch (scan->get_token(a)) {
                        case token::invalid:

                        case token::class_int:     //int

                        case token::class_double:  //double
                        case token::class_string:  //string
                        case token::class_char:    //char
                        case token::bit_and: //&
                        case token::bit_or:  //|
                        case token::bit_not: //~
                        case token::log_and: //&&
                        case token::log_or:  //||
                        case token::log_not: //~
                        case token::plus:   //+
                        case token::minus:  //-
                        case token::times:  //*
                        case token::div:    ///
                        case token::mod:    //%
                        case token::equ:        //==
                        case token::not_equ:       //!=
                        case token::great_equ:  //>=
                        case token::less_equ:   //<=
                        case token::great:      //>
                        case token::less:       //<
                        case token::assign:     //=
                        case token::plus_agn:   //+=
                        case token::minus_agn:  //-=
                        case token::times_agn:  //*=
                        case token::div_agn:    ///=
                        case token::mod_agn:    //%=
                        case token::l_par:       //(
                        case token::r_par:       //)
                        case token::l_mid_par:   //[
                        case token::r_mid_par:   //]
                        case token::l_big_par:   //{
                        case token::r_big_par:   //}
                        case token::comma:   //,
                        case token::comment: //#
                        case token::indentif:  //标识符
                        case token::key_if:         //if

                        case token::key_else:       //else
                        case token::key_true:       //true
                        case token::key_false:      //false
                        case token::key_while:      //while
                        case token::key_for:        //for
                        case token::key_null:       //null
                        case token::key_func:       //func
                        case token::key_return:     //return
                        case token::key_break:      //break
                        case token::key_continue:   //continue

                        default:
                        break;
                }
            }
        }
};