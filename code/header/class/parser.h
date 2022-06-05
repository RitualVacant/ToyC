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

        std::string file_path;         //初始化scanning
        std::string output_file_path;
        bool really_output_asm_code = false;

        std::vector<statement> code;
        std::size_t lable = 0;

        //TODO
        //symbol_table table = symbol_table(output_file_path, really_output_asm_code);
        symbol_table* table;
        scanning     scan  = scanning(file_path);
        synctax_tree tree  = synctax_tree();
        //std::unique_ptr<asm_code> asm_file  = std::make_unique<asm_code>(asm_code(fil))
        asm_code* asm_file = nullptr;
        //
        bool is_func;
        bool is_struct;

        //临时变量的个数,类似会累加的闭包,parser::get_var_time()
        std::size_t var_time = 0;

        //void parser_if_statement ();
        //void parser_while_statement();
        //std::string parser_return_statement();

        //解析----没有完工
        //void parser_statement();
        //void parser_function_define_or_declare(std::string func_name, token func_return_type);
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
        //DROP
        void parser_declare_or_function_define_or_declare();

        //NEW
        //declare
        ast::ptr parser_declare_or_definition();
        ast::ptr parser_declaration_declarator();
        ast::ptr parser_initial_declarator_list();
        ast::ptr parser_initial_declarator();
        ast::ptr parser_declarator();
        ast::ptr parser_direct_declarator();
        ast::ptr parser_arguments_arguments_type_list();
        ast::ptr parser_arguments_list();
        ast::ptr parser_arugments_declaration();
        ast::ptr parser_array_declarator();
        ast::ptr parser_temporary_1();
        //NEW
        //expression
        //void parser_expression();
        ast::ptr parser_assignment_expression();
        ast::ptr parser_conditional_expression();
        ast::ptr parser_binary_expression();
        ast::ptr parser_unary_expression();
        ast::ptr parser_postfix_expression();
        ast::ptr parser_postfix_operator();
        ast::ptr parser_primary_expression();
        //NEW
        //statement
        ast::ptr parser_compound_statement();
        ast::ptr parser_block_list();
        ast::ptr parser_block();
        ast::ptr parser_mark_statement();
        ast::ptr parser_statement();
        ast::ptr parser_mark();
        ast::ptr parser_constant_expression();
        ast::ptr parser_not_mark_statement();
        ast::ptr parser_if_statement();
        ast::ptr parser_swtich_statement();
        ast::ptr parser_do_while_statement();
        ast::ptr parser_while_statement();
        ast::ptr parser_for_statement();
        ast::ptr parser_break_statement();
        ast::ptr parser_goto_statement();
        ast::ptr parser_continue_statement();
        ast::ptr parser_return_statement();

    public:
        explicit parser(std::string &file_path_, std::string &output_file_path_, bool really_output_asm_code_);
        ~parser();
        parser(parser const&)       = delete;
        parser(parser&&)            = delete;
        parser& operator=(parser&)  = delete;
        parser& operator=(parser&&) = delete;

        void print_mid_code();
        void print_asm_code_code();
        void print_synctax_tree();
};


 parser::parser(std::string &file_path_, std::string &output_file_path_, bool really_output_asm_code_)
: file_path(file_path_), output_file_path(output_file_path_), really_output_asm_code(really_output_asm_code_) {
    scan.next_token();
    while (!scan.file.eof()) {
        ast::ptr ptr_root = parser_declare_or_definition();
        tree.connect(ptr_root);
    }
};

parser::~parser() {
    //delete scan;
    delete asm_file;
};

#endif