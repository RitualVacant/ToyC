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

class sign_map {
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

        std::vector<func> func_table;
        std::vector<frame> symbol_stack;//符号表

    public:
        std::fstream file;
        std::string file_path;


        explicit sign_map(std::string &file_path_);
        ~sign_map();
        sign_map(sign_map&)             = delete;
        sign_map(sign_map&&)            = delete;
        sign_map& operator=(sign_map&)  = delete;
        sign_map& operator=(sign_map&&) = delete;

    //符号表
        bool find(std::tuple<token, std::string> &x);
        bool find(std::string &x);
        std::size_t at(std::tuple<token, std::string> &x);
        std::size_t at(std::string &x);
        bool find_func(std::string& name);
        bool find_argu(std::string name);
        std::string find_local(std::string &name);

        void push_var(frame x);
        void clear_symbol_stack();
        token find_var_class(std::string name);

        void push_global_sign(std::string &x);

        void insert_func(func);
        void push_func();
        void pop_func();
        std::string find_func_return_var_value(std::string func_name);
};

sign_map::sign_map(std::string &file_path_) : file_path(file_path_) {
    file.open(file_path, std::ofstream::out);
    if (!file.is_open()) {
            fmt::print(fg(fmt::color::red), "\nfail to open the file!\n");
            fmt::print(fg(fmt::color::red), "please check the output file's path\n");
            fmt::print(fg(fmt::color::red), "--end--\n");
            exit(1);
    }
    //初始化
}

sign_map::~sign_map() {
    file.close();
}

#endif