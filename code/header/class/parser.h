#ifndef PARSER_H
#define PARSER_H

#pragma once
#include <string>
#include <fstream>
#include <tuple>
#include <memory>
#include <stdlib.h>
#include "./../AST/token.h"
#include "./../AST/ast.h"
#include "./../../scanning.cpp"
#include "./../../symbol_table.cpp"
#include "./../../worng.cpp"
#include "./../../asm_code.cpp"
#include "./../../synctax_tree.cpp"


class parser {
    private:
        //yes or no
        bool really_output_asm_code = false;

        std::string file_path;         //初始化scanning
        std::string output_file_path;

        std::vector<statement> code;
        std::size_t lable = 0;

        std::unique_ptr<symbol_table> table = std::make_unique<symbol_table>(symbol_table(output_file_path, really_output_asm_code));
        std::unique_ptr<scanning> scan      = std::make_unique<scanning>(scanning(file_path));
        //std::unique_ptr<synctax_tree> tree  = std::make_unique<synctax_tree>();
        synctax_tree tree;
        //std::unique_ptr<asm_code> asm_file  = std::make_unique<asm_code>(asm_code(fil))
        asm_code* asm_file = nullptr;
        //
        bool is_func;

        //临时变量的个数,类似会累加的闭包,parser::get_var_time()
        std::size_t var_time = 0;

        //void parser_if_statement ();
        //void parser_while_statement();
        //std::string parser_return_statement();

        //解析----没有完工
        //void parser_statement();
        void parser_function_define_or_declare(std::string func_name, token func_return_type);
        void parser_expression_(std::string l_value);
        //TODO这两个函数
        std::string parser_expression();
        std::string parser_unit(std::string name_array);
        //void parser_primary_expression();
        void parser_lvalue();
        //TODO add support for pointer and array declare;
        //TODO now don't support pointer and array
        //void parser_declare();
        void parser_expression_unit();
        std::string parser_func_call(std::string name_func);
        std::string get_var_time();
        std::string get_lable();

        //前缀表达式转后缀表达式
        //本应该写在函数中，在类中减少内存反复分配
        std::vector<std::tuple<token, std::string>> parser_pre_to_pos();
        void parser_declare_or_function_define_or_declare();

        //NEW
        //declare
        void parser_declare_or_function_definition();
        void parser_declaration_declarator();
        void parser_initial_declarator_list();
        void parser_initial_declarator();
        void parser_declarator();
        void parser_direct_declarator();
        void parser_arguments_arguments_type_list();
        void parser_arguments_list();
        void parser_arugments_declaration();
        void parser_array_declarator();
        void parser_temporary_1();
        //NEW
        //expression
        //void parser_expression();
        void parser_assignment_expression();
        void parser_conditional_expression();
        void parser_binary_expression();
        void parser_unary_expression();
        void parser_postfix_expression();
        void parser_postfix_operator();
        void parser_primary_expression();
        //NEW
        //statement
        void parser_compound_statement();
        void parser_block_list();
        void parser_block();
        void parser_mark_statement();
        void parser_statement();
        void parser_mark();
        void parser_constant_expression();
        void parser_not_mark_statement();
        void parser_if_statement();
        void parser_swtich_statement();
        void parser_do_while_statement();
        void parser_while_statement();
        void parser_for_statement();
        void parser_break_statement();
        void parser_goto_statement();
        void parser_continue_statement();
        void parser_return_statement();

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
    //scan = new scanning(file_path);
    //table = new symbol_table(output_file_path, really_output_asm_code);
    while (!scan->file.eof()) {
        parser_declare_or_function_define_or_declare();
    }
    if (really_output_asm_code) {
        table->init_loc_argu();
        //asm_file = new asm_code(output_file_path, std::move(code), table);
    }
};

parser::~parser() {
    //delete scan;
    delete asm_file;
};

#endif