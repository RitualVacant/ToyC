#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef TUPLE
#define TUPLE
#include <tuple>
#endif

#ifndef TOKEN
#define TOKEN
#include "./AST/token.h"
#endif

#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

#ifndef AST
#define AST
#include "./AST/ast.h"
#endif

class parser {
    private:
        unsigned int line = 0;
        unsigned int column = 0;
        char c;
        std::size_t num_token = 0;  //the number of token in this script file
        std::string file_path;
        std::fstream file;
        std::tuple<token, std::string> &&now_token = {};

        std::tuple<token, std::string> next_token();
        void get_next_char();

        std::tuple<token, std::string> to_number();
        std::tuple<token, std::string> to_keyWord_or_indentif();
        std::tuple<token, std::string> to_comment();
        std::tuple<token, std::string> to_char();
        std::tuple<token, std::string> to_string();

        token get_token(std::tuple<token, std::string> &tuple_);
        std::string get_value(std::tuple<token, std::string> &tuple_);

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
        explicit parser(std::string &file_path_) : file_path (file_path_) {
            file.open(file_path);
            if (!file.is_open()) {
                std::cout
                << "fail to open the file!" << std::endl
                << "please check the input file's path" << std::endl
                << "--end--" << std::endl;
                exit(1);
            }
            c = file.get();
        };
        ~parser() {
            file.close();
        };

        //测试词义分析器函数
        void test_output() {
            while (!file.eof()) {
                auto &&a = next_token();
                if (test_output_token.find(std::get<0>(a)) == test_output_token.end())
                    std::cout << "indentif " << std::get<1>(a) << std::endl;
                else
                    std::cout << test_output_token.at(std::get<0>(a)) << ' ' << std::get<1>(a) << std::endl;
            }
        }
        void create_node() {
            while (!file.eof()) {
                auto &&a = next_token();
                switch (get_token(a)) {
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