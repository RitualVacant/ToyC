#ifndef ASM_FILE_H
#define ASM_FILE_H

#pragma once
#include <fstream>
#include "./../../parser.cpp"

struct stack_frame {
    std::string name;
    token class_var;
    std::size_t offset;
    std::size_t size_total;
    std::size_t size_unit;
    std::string location;
    stack_frame(
                std::string name_, token class_var_,
                std::size_t offset_, std::size_t size_total_,
                std::size_t size_unit_
               ) :
        name(name_), class_var(class_var_),
        offset(offset_), size_total(size_total_),
        size_unit(size_unit_)
        {
            location = fmt::format("[bp-{}]", offset);
        };
};

class asm_file {
    private:
        std::string file_path;
        std::fstream file;
        std::vector<statement> code;

        std::vector<stack_frame> symbol_stack;
        std::vector<func> func_table;
        std::size_t sp = 0;

    public:
        explicit asm_file(std::string &file_path);
        ~asm_file();
        asm_file(std::string const &)       = delete;
        asm_file(std::string &&)            = delete;
        asm_file& operator=(std::string &)  = delete;
        asm_file& operator=(std::string &&) = delete;

        void get_code(std::vector<statement> &code, std::vector<func> &func_table);
        void read_code();

        void push(stack_frame x);
        func& find_func(std::string name);
        std::string find_var_location(std::string name);
        void asm_write_code(std::string symbol, std::string arg1, std::string arg2);
        void asm_write_lable(std::string lable);
        void asm_write_mov(std::size_t size, std::string arg1, std::string arg2);
};

asm_file::asm_file(std::string &file_path_) :
    file_path(file_path_)
{
    file.open(file_path, std::ofstream::out);
};

asm_file::~asm_file() {
    file.close();
};

#endif