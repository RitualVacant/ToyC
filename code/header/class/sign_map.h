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

//寄存器号数
std::unordered_map<std::size_t, std::string> register_name {
    {0, "rdi"},
    {1, "rsi"},
    {2, "rdx"},
    {3, "rcx"},
    {4, "r8"},
    {5, "r9"},
};

//栈帧
struct frame {
    token Token;
    std::string name;
    std::size_t offset;
    explicit frame(token Token_, std::string name_, std::size_t offset_) :
        Token(Token_),
        name(name_),
        offset(offset_)
    {};
};
//函数表
struct func {
    std::string name;
    std::vector<std::tuple<token, std::string>> argu;
    token class_reture;

    func(std::string &name_, std::vector<std::tuple<token, std::string>> &argu_, token &class_reture_) :
        name(name_),
        argu(argu_),
        class_reture(class_reture_)
        {};
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

        //std::vector<std::tuple<token, std::string>> sign_stack;
        std::vector<frame> sign_stack;
        //TODO 这里必须带token不然没有办法搞清楚空间
        std::unordered_set<std::string> global_sign;
        //std::unordered_set<std::string> func_set;
        std::vector<func> func_table;
        //函数现在的参数
        std::vector<std::tuple<token, std::string>> argu_table;

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
        token find_var_class(std::string name);

        void push_token(std::tuple<token, std::string> x);
        void push_global_sign(std::string &x);

        void insert_func(func);
        void push_func();
        void pop_func();

    //asm文件读写
        void asm_init();
        void asm_argu_push();
        void asm_tem_var_push(std::size_t size);
        void asm_func_head(std::string func_name);
        void asm_func_end();

        std::string asm_ptr_size(std::size_t size);
        void asm_any_auto_r__(token which, std::size_t size, std::string source, std::string option);
        void asm_any_auto___r(token which, std::size_t size, std::string source, std::string option);

        void asm_mov(std::size_t size, std::string source, std::string option);
        void asm_mov(std::string source, std::string option);
        void asm_mov_auto_r__(std::size_t size, std::string source, std::string option);
        void asm_mov_auto___r(std::size_t size, std::string source, std::string option);

        void asm_add(std::size_t size, std::string source, std::string option);
        void asm_add_auto_r__(std::size_t size, std::string source, std::string option);
        void asm_add_auto___r(std::size_t size, std::string source, std::string option);

        void asm_sub(std::size_t size, std::string source, std::string option);
        void asm_sub_auto_r__(std::size_t size, std::string source, std::string option);
        void asm_sub_auto___r(std::size_t size, std::string source, std::string option);

        void asm_mul(std::size_t size, std::string source, std::string option);
        void asm_div(std::size_t size, std::string source, std::string option);
        char asm_register_pre(std::size_t size);
};

sign_map::sign_map(std::string &file_path_) : file_path(file_path_) {
    file.open(file_path);
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