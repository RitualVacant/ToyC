#ifndef ASM_FILE_H
#define ASM_FILE_H

#pragma once
#include <fstream>
#include "./../../parser.cpp"

//stack_frame of asm_code
//in asm_code.h
//all info
struct stack_frame {
    std::string name;
    token class_var;
    std::size_t offset;
    std::size_t size_total;
    std::size_t size_unit;
    std::string location;

    stack_frame() = default;
    stack_frame (
        std::string name_,
        token class_var_,
        std::size_t offset_,
        std::size_t size_total_,
        std::size_t size_unit_
    ):
    name{name_},
    class_var{class_var_},
    offset{offset_},
    size_total{size_total_},
    size_unit{size_unit_} {
        location = fmt::format("[bp-{}]", offset);
    };
};

enum class register_name : std::size_t{
    rax,
    rbx,
    rcx,
};
std::size_t const sizeof_register_used = sizeof(register_name) / sizeof(std::size_t);

class asm_code {
    private:
        std::string file_path;
        std::fstream file;
        std::vector<statement> code;

        //符号表，是parser中的拷贝
        symbol_table* table {nullptr};
        //如果有符号表的拷贝，那么弃用
        std::vector<stack_frame> symbol_stack;
        //如果有符号表的拷贝，那么弃用
        //std::vector<func> func_table;
        //
        std::size_t sp = 0;
        //DROP 符号表已经有这个功能sign register is used
        bool register_used[sizeof_register_used];

    public:
        explicit asm_code(std::string &file_path, std::vector<statement> &&code,symbol_table* sign_);
        ~asm_code();
        asm_code(std::string const &)       = delete;
        asm_code(std::string &&)            = delete;
        asm_code& operator=(std::string &)  = delete;
        asm_code& operator=(std::string &&) = delete;

        //DROP现在构造函数中已经初始化
        void get_code(std::vector<statement> &code, std::vector<func> &func_table);
        //对中间代码进行翻译
        void read_code();

        //统计函数内所有变量占用的空间
        std::size_t sizeof_all_var_in_func(std::size_t now_loc);
        //

        //
        void push(stack_frame x);
        //
        func& find_func(std::string name);
        //找到变量的位置
        std::string find_var_location(std::string name);
        //返回数组，立即数，变量的地址
        std::string trans_to_loc(std::string name);
        //
        //DROP寄存器使用情况
        bool register_is_used(register_name);
        //
        void asm_code_write_code(std::string symbol, std::string arg1, std::string arg2);
        void asm_code_write_lable(std::string lable);
        void asm_code_write_mov(std::size_t size, std::string arg1, std::string arg2);
};

asm_code::asm_code(std::string &file_path_, std::vector<statement> &&code_, symbol_table* sign_)
: file_path{file_path_}, code{code_} ,table{sign_} {
    file.open(file_path, std::ofstream::out);
    if (!file.is_open()) {
        fmt::print("can't open file at {}", file_path);
        exit(0);
    }
    read_code();
};

asm_code::~asm_code() {
    file.close();
};

#endif