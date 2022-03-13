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
//
std::unordered_map<token, std::size_t> size_class{
    {token::class_int,    4},
    {token::class_char,   1},
    {token::class_double, 4},
    {token::class_bool,  1}
};

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
            /*
            按道理应该这样写的,c++标准库居然不支持塞这种类，不能生成哈希值
            只能塞string了，坑死人
            std::unordered_set<std::tuple<token, std::string>> global_sign;
            */
           //TODO 这里必须带token不然没有办法搞清楚空间
        std::unordered_set<std::string> global_sign;
        std::unordered_set<std::string> func_set;


    public:
        std::fstream file;
        std::string file_path;



        explicit sign_map(std::string &file_path_);
        ~sign_map();
        sign_map(sign_map&)             = delete;
        sign_map(sign_map&&)            = delete;
        sign_map& operator=(sign_map&)  = delete;
        sign_map& operator=(sign_map&&) = delete;

        void set_info(std::size_t line, std::size_t colume, token now_token);//设置方便报错

    //符号表
        bool find(std::tuple<token, std::string>);
        bool find_func(std::string);

        void push_token(std::tuple<token, std::string> x);
        void push_global_sign(std::string x);

        void push_func(std::tuple<token, std::string> x);
        void pop_func();

    //asm文件读写
        void asm_argu_push();
        void asm_tem_var_push(std::size_t size, std::string value);
        void asm_func_head(std::string func_name);
        void asm_func_end();
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