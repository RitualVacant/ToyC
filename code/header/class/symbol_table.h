#ifndef SIGN_MAP_H
#define SIGN_MAP_H

#pragma once
#include <vector>
#include <tuple>
#include <unordered_set>
#include "./../AST/token.h"
#include <string>
#include "./../../parser.cpp"
#include <unordered_map>

//类型大小
std::unordered_map<token, std::size_t> size_class{
    {token::key_int,    4},
    {token::key_char,   1},
    {token::key_double, 4},
    {token::key_bool,   1},
    {token::class_int,    4},
    {token::class_char,   1},
    {token::class_double, 4},
    {token::class_bool,   1}
};

struct statement {
    token symbol;
    std::string arg1;
    std::string arg2;
    std::string result;
    statement(token symbol_, std::string arg1_, std::string arg2_, std::string result_) :
        symbol(symbol_), arg1(arg1_), arg2(arg2_), result(result_) {};
};

struct arg_info {
    token arg_class;
    std::string name;
    std::string location;
    arg_info(token arg_class_, std::string name_, std::string location_) :
        arg_class(arg_class_), name(name_), location(location_) {};
};

//栈帧
struct frame {
    token var_class;
    std::string name;
    explicit frame(token var_class_, std::string name_) :
        var_class(var_class_), name(name_) {};
};

//函数表
struct func {
    std::string name;
    std::vector<arg_info> argu;
    token class_reture;
    std::string name_return;

    func(std::string &name_, std::vector<arg_info> &argu_, token &class_reture_, std::string &name_return_) :
        name(name_), argu(argu_), class_reture(class_reture_), name_return(name_return_) {};
};

class symbol_table {
    private:
    //
    //符号表部分
    //

        std::size_t bp = 0;
        std::size_t sp = 0;

        //获取line colume
        std::size_t line;
        std::size_t colume;
        token now_token;

        std::vector<frame> sign_stack;
        std::unordered_set<std::string> global_sign;

        //貌似已经被弃用，符号表
        std::vector<frame> symbol_stack;//符号表
        //传参时在寄存器中的参数的寄存器列表
        std::vector<std::string> argu_register_loc = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

    public:
        //函数表
        std::vector<func> func_table;

        std::fstream file;
        std::string file_path;


        explicit symbol_table(std::string &file_path_, bool really_output_asm_code_);
        ~symbol_table();
        symbol_table(symbol_table&)             = delete;
        symbol_table(symbol_table&&)            = delete;
        symbol_table& operator=(symbol_table&)  = delete;
        symbol_table& operator=(symbol_table&&) = delete;

    //符号表
        //DROP在栈中寻找
        bool find(std::tuple<token, std::string> &x);
        //DROP在栈中寻找
        bool find(std::string &x);
        //DROP返回在栈中的偏移量
        std::size_t at(std::tuple<token, std::string> &x);
        //DROP返回在栈中的偏移量
        std::size_t at(std::string &x);
        //DROP在函数参数表中寻找
        bool find_argu(std::string name);
        //DROP返回变量，或者是函数参数在汇编中的字符串表达
        std::string find_local(std::string &name);

        //在函数表中寻找函数
        bool find_func(std::string& name);

        void push_var(frame x);
        void clear_symbol_stack();
        token find_var_class(std::string name);

        void push_global_sign(std::string &x);

        //把函数插入函数表,TODO并把now_arge替换成这个函数的参数表
        void insert_func(func);

        //void push_func();
        //DROP把这个函数的所有东西出栈，包括参数
        void pop_func();
        std::string find_func_return_var_value(std::string func_name);
        //初始化函数传参的位置的位置 parser.cpp
        void init_loc_argu();
};

symbol_table::symbol_table(std::string &file_path_, bool really_output_asm_code_)
: file_path(file_path_) {
    if (really_output_asm_code_) return;

    file.open(file_path, std::ofstream::out);
    if (!file.is_open()) {
        fmt::print(fg(fmt::color::red), "\nfail to open the file!\n");
        fmt::print(fg(fmt::color::red), "please check the output file's path\n");
        fmt::print(fg(fmt::color::red), "--end--\n");
        exit(1);
    }
}

symbol_table::~symbol_table() {
    file.close();
}

#endif