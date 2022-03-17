#ifndef ASM_FILE_CPP
#define ASM_FILE_CPP

#pragma once
#include "./header/class/asm_file.h"
#include <sstream>


func& asm_file::find_func(std::string name) {
    for (auto &i : func_table) {
        if (i.name == name)
            return i;
    }
    fmt::print("can't find func int func_table in asm_file::find_func");
    exit(0);
    return;
}

//寄存器号数
std::vector<std::string> register_name {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void asm_file::get_code(std::vector<frame> &code_, std::vector<func> &func_table_) {
    code = std::move(code_);
    func_table = std::move(func_table_);

    //offset of argu in stack
    std::size_t offset = 0;
    //init argu location
    for (auto &i : func_table) {
        for (std::size_t j = 0; j < i.argu.size(); ++j) {
            if (j < 6) {
                i.argu[j].location = register_name[j];
            }
            //TODO
            else {
                switch (i.argu[j].arg_class) {
                case token::class_int: {
                }
                case token::class_double:
                case token::class_char:
                case token::class_bool:
                break;
                default:
                    fmt::print("\na unknow error at asm_file::get_code\n");
                    break;
                }
            }
        }
    }
    return;
}

void asm_file::push(stack_frame x) {
    sp += x.size_total;
    x.offset = sp;
    symbol_stack.push_back(x);
    return;
}

void asm_file::asm_write(std::string &symbol, std::string &arg1, std::string &arg2) {
    file << fmt::format("   {:8}{:8}{:8}\n", symbol, arg1, arg2);
    return;
}

void asm_file::asm_write_lable(std::string &lable) {
    file << fmt::format("{}:\n", lable);
    return;
}

void asm_file::asm_write_mov(std::size_t size, std::string &arg1, std::string &arg2) {
    switch (size) {
        case 1:
            file << fmt::format("   {:8} byte {:8},{:8}", "mov", arg1, arg2);
            return;
        case 2:
            file << fmt::format("   {:8} word {:8},{:8}", "mov", arg1, arg2);
            return;
        case 4:
            file << fmt::format("   {:8} dword {:8},{:8}", "mov", arg1, arg2);
            return;
        case 8:
            file << fmt::format("   {:8} dword {:8},{:8}", "mov", arg1, arg2);
            return;
        default:
            fmt::print("\na unknow error at asm_file::asm_write_mov()\n");
            exit(0);
    }
    return;
}

void asm_file::read_code() {
    for (std::size_t i = 0; i < code.size(); ++i) {
        switch (code[i].symbol) {
            token::invalid: {
                fmt::print("\na unknow error at asm_file::read_code()\n");
                exit(0);
            }
            token::jmp: {
                asm_write_code("jmp", code[i].arg1, "");
                continue;
            }
            token::lable: {
                asm_write_lable(code[i].arg1);
                continue;
            }
            token::dec_int: {
                push(stack_frame(code[i].arg1, token::int_class, sp + 4, 4, 4));
                asm_write_code("sub", "sp", "4");
                continue;
            }
            token::dec_double: {
                push(frame(code[i].arg1, token::double_class, sp + 4, 4, 4));
                asm_write_code("sub", "sp", "4");
                continue;
            }
            token::dec_char: {
                push(frame(code[i].arg1, token::char_class, sp + 1, 1, 1));
                asm_write_code("sub", "sp", "1");
                continue;
            }
            token::dec_bool: {
                push(frame(code[i].arg1, token::bool_class, sp + 1, 1, 1));
                asm_write_code("sub", "sp", "1");
                continue;
            }
            token::dec_int_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 4;
                push(frame(code[i].arg1, token::class_int_arrary, sp + size_array, size_array, 4));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_double_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 4;
                push(frame(code[i].arg1, token::class_int_arrary, sp + size_array, size_array, 4));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_char_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 1;
                push(frame(code[i].arg1, token::class_char_arrary, sp + size_array, size_array, 1));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_bool_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 1;
                push(frame(code[i].arg1, token::class_bool_arrary, sp + size_array, size_array, 1));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_func: {
                asm_write_lable(code[i].arg1);
                asm_write_code("push", "rbp", "");
                asm_write_code("mov", "rbp", "rsp");
                continue;
            }
            token::call_func: {
                std::string name_func = code[i].arg1;
                func &now_func = find_func();
                for (; code[i].symbol != token::call_func_end; ++i) {

                }
            }
            token::call_func_end: {
                asm_write_code("call", code[i].arg1, "");
            }
            token::func_return: {
                asm_write_code("mov", "rsp", "rbp");
                asm_write_code("pop", "rbp", "");
                asm_write_code("ret", "", "");
            }
            token::arg_r_int: {

            }
            token::arg_r_double: {

            }
            token::arg_r_char {         //*函数参数调用立即数
            }
            token::arg_r_bool           //*函数参数调用立即数
            token::arg_int_class      //*函数参数调用
            token::arg_double_class   //*函数参数调用
            token::arg_char_class     //*函数参数调用
            token::arg_bool_class     //*函数参数调用

            token::class_int     //int
            token::class_double  //double
            token::class_char    //char
            token::class_bool    //bool

            token::class_int_array     //int array
            token::class_double_array  //double array
            token::class_char_array    //char array
            token::class_bool_array

            token::class_int_array_unit
            token::class_double_array_unit
            token::class_char_array_unit
            token::class_bool_array_unit

            token::arrary_unit //TODO 数组运算到底要不要分出类型， 这里暂时不分用这个代替
            token::
            token::key_void
            token::key_int
            token::key_double
            token::key_string
            token::key_char
            token::key_bool

            token::r_int
            token::r_double
            token::r_char
            token::r_bool

            token::bit_and //&
            token::bit_or  //|
            token::bit_not //~

            token::log_and //&&
            token::log_or  //||
            token::log_not //~

            token::plus   //+
            token::minus  //-
            token::times  //*
            token::div    ///
            token::mod    //%

            token::ver        //->
            token::equ        //==
            token::not_equ    //!=
            token::great_equ  //>=
            token::less_equ   //<=
            token::great      //>
            token::less       //<

            token::assign     //=
            token::plus_agn   //+=
            token::minus_agn  //-=
            token::times_agn  //*=
            token::div_agn    ///=
            token::mod_agn    //%=

            token::l_par       //(
            token::r_par       //)
            token::l_mid_par   //[
            token::r_mid_par   //]
            token::l_big_par   //{
            token::r_big_par   //}

            token::comma   //,
            token::comment //#
            token::end     //;

            token::indentif  //标识符
            token::key_if         //if
            token::key_else       //else
            token::key_true       //true
            token::key_false      //false
            token::key_while      //while
            token::key_for        //for
            token::key_null       //null
            token::key_func       //func
            token::key_return     //return
            token::key_break      //break
            token::key_continue   //continue
        }
    }
}

#endif