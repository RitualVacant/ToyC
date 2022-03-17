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
    stack_frame(
                std::string name_, token class_var_,
                std::size_t offset_, std::size_t size_total_,
                std::size_t size_unit_
               ) :
        name(name_),
        class_var(class_var_),
        offset(offset_),
        size_total(size_total_),
        size_unit(size_unit_)
        {};
};

class asm_file {
    private:
        std::string file_path;
        std::fstream file;
        std::vector<statement> &&code;

        std::vector<frame> symbol_stack;
    public:
        asm_file(std::string file_path);
        ~asm_file();
        asm_file(std::string &file_path)             = delete;
        asm_file(std::string &&file_paht)            = delete;
        asm_file& operator=(std::string &file_path)  = delete;
        asm_file& operator=(std::string &&file_path) = delete;

        void get_code(std::vector<statement> &code);
        void read_code();
        void push(frame);
};

asm_file::asm_file(std::string file_path_) :
    file_path(file_path_)
{
    file.open(file_path, std::ofstream::out);
};

asm_file::~asm_file() {
    file.close();
};

#endif