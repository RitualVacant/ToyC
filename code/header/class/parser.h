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
#include "./../../asm_file.cpp"



class parser {
    private:
        std::string file_path;         //初始化scanning
        std::string output_file_path;

        std::vector<statement> code;
        std::size_t lable = 0;

        //符号表 RAII
        sign_map* sign;
        //词法分析 RAII
        scanning* scan;
        //asm
        asm_file* asm_code;

        //临时变量的个数, 会累加的闭包
        std::size_t var_time = 0;

        void parser_if_statement ();
        void parser_while_statement();
        std::string parser_return_statement();

        //解析----没有完工
        void parser_statement();
        void parser_function();
        void parser_expression_(std::string l_value);
        //TODO这两个函数
        std::string parser_expression();
        std::string parser_unit(std::string name_array);
        void parser_primary_expression();
        void parser_lvalue();
        void parser_declare();
        void parser_expression_unit();
        std::string parser_func_call(std::string name_func);

        std::vector<std::tuple<token, std::string>> parser_pre_to_pos();

        std::string get_var_time();
        std::string get_lable();


    public:
        explicit parser(std::string &file_path_, std::string &output_file_path_);
        ~parser();
        parser(parser const&)       = delete;
        parser(parser&&)            = delete;
        parser& operator=(parser&)  = delete;
        parser& operator=(parser&&) = delete;

        void print_mid_code();
        void print_asm_code();
};


parser::parser(std::string &file_path_, std::string &output_file_path_)
   :file_path(file_path_), output_file_path(output_file_path_)
{
    scan = new scanning(file_path);
    sign = new sign_map(output_file_path);
    asm_code = new asm_file(output_file_path);
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