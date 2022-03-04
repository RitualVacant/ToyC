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

#ifndef SIGN_MAP
#define SIGN_MAP
#include "./../../sign_map.cpp"
class parser {
    private:
        unsigned int line = 0;
        unsigned int column = 0;
        std::tuple<token, std::string> &&now_token = {};
        std::size_t num_token = 0;  //the number of token in this script file
        std::string file_path;
        std::string output_file_path;
        std::fstream output_file;

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
        void create_node();

        //输出语法分析中的错误信息且终止进程
        void output_error_message(token);

    public:
        //parser(std::string file_path_) : file_path(file_path_) {};
        explicit parser(std::string &file_path_, std::string &output_file_path_) :
            file_path(file_path_), output_file_path(output_file_path_) {

            scan = new scanning(file_path);

            //output_file
            output_file.open(output_file_path);
            if (output_file.is_open()) {
                fmt::print("the output file's path is worng! \n plase check the path");
                exit(0);
            }
            //main funcion jmp;
            output_file << "jmp main\n";
        };

        ~parser() {
            delete scan;
        };

        parser(parser const&)       = delete;
        parser(parser&&)            = delete;
        parser& operator=(parser&)  = delete;
        parser& operator=(parser&&) = delete;

};