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
#include "./../../symbol_list.cpp"
#include "./../../worng.cpp"
#include "./../../asm_code.cpp"



class parser {
    private:
        //yes or no
        bool really_output_asm_code = false;

        std::string file_path;         //初始化scanning
        std::string output_file_path;

        std::vector<statement> code;
        std::size_t lable = 0;

        //符号表 RAII
        symbol_list* list = nullptr;
        //词法分析 RAII
        scanning* scan = nullptr;
        //asm_code
        asm_code* asm_file = nullptr;

        //临时变量的个数,类似会累加的闭包,parser::get_var_time()
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

        //前缀表达式转后缀表达式
        std::vector<std::tuple<token, std::string>> parser_pre_to_pos();

        std::string get_var_time();
        std::string get_lable();


    public:
        explicit parser(std::string &file_path_, std::string &output_file_path_, bool really_output_asm_code_);
        ~parser();
        parser(parser const&)       = delete;
        parser(parser&&)            = delete;
        parser& operator=(parser&)  = delete;
        parser& operator=(parser&&) = delete;

        void print_mid_code();
        void print_asm_code_code();
};


parser::parser(std::string &file_path_, std::string &output_file_path_, bool really_output_asm_code_)
: file_path(file_path_), output_file_path(output_file_path_), really_output_asm_code(really_output_asm_code_) {
    scan = new scanning(file_path);
    list = new symbol_list(output_file_path, really_output_asm_code);
    while (!scan->file.eof()) {
        parser_statement();
    }
    if (really_output_asm_code) {
        list->init_loc_argu();
        asm_file = new asm_code(output_file_path, std::move(code), list);
    }
};

parser::~parser() {
    delete scan;
    delete list;
    delete asm_file;
};

#endif