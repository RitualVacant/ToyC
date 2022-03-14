#ifndef PARSER_H
#define PARSER_H

#pragma once
#include <string>
#include <fstream>
#include <tuple>
#include "./../AST/token.h"
#include <stdlib.h>
#include "./../AST/ast.h"
#include "./../../scanning.cpp"
#include "./../../sign_map.cpp"
#include "./../../worng.cpp"


class parser {
    private:
        std::tuple<token, std::string> &&now_token = {};
        std::size_t num_token = 0;  //the number of token in this script file
        std::string file_path; //初始化scanning
        std::string output_file_path;

        //符号表 RAII
        sign_map* sign;
        //词法分析 RAII
        scanning* scan;

        //词法分析器

        //创造节点
        void parser_if_statement ();
        void parser_while_statement();
        void parser_return_statement();

        //解析----没有完工
        void parser_statement();
        void parser_function();
        void parser_expression(std::size_t size);
        void parser_unary_expression();
        void parser_primary_expression();
        void parser_lvalue();
        void parser_declare();

        void parser_pre_to_pos();

    public:
        //parser(std::string file_path_) : file_path(file_path_) {};
        explicit parser(std::string &file_path_, std::string &output_file_path_);
        ~parser();
        parser(parser const&)       = delete;
        parser(parser&&)            = delete;
        parser& operator=(parser&)  = delete;
        parser& operator=(parser&&) = delete;
};


parser::parser(std::string &file_path_, std::string &output_file_path_)
   :file_path(file_path_), output_file_path(output_file_path_)
{
    scan = new scanning(file_path);
    sign = new sign_map(output_file_path);
    //TODO 这里要调用
    while (!scan->file.eof()) {
        parser_statement();
    }
};

parser::~parser() {
    delete scan;
    delete sign;
};

#endif