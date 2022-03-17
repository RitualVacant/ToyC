#ifndef ASM_FILE_CPP
#define ASM_FILE_CPP

#pragma once
#include "./header/class/asm_file.h"
#include <sstream>

void asm_file::get_code(std::vector<frame> &code_) {
    code = std::move(code_);
    return;
}

void asm_file::push(stack_frame x) {
    offset += x.size_total;
    symbol_stack.push_back(x);
    return;
}

void asm_file::asm_write(std::sting &symbol, std::string &arg1, std::string &arg2) {
    file << fmt::format({:6} {:6} {:6}, symbol, arg1, arg2);
    return;
}

void asm_write_lable(std::string &lable) {
    file << fmt::format("{}:", lable);
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
                push(stack_frame(code[i].arg1, token::int_class, offset, 4, 4));
                asm_write_code("sub", "sp", "4");
                continue;
            }
            token::dec_double: {
                push(frame(code[i].arg1, token::double_class, offset, 4, 4));
                asm_write_code("sub", "sp", "4");
                continue;
            }
            token::dec_char: {
                push(frame(code[i].arg1, token::char_class, offset, 1, 1));
                asm_write_code("sub", "sp", "1");
                continue;
            }
            token::dec_bool: {
                push(frame(code[i].arg1, token::bool_class, offset, 1, 1));
                asm_write_code("sub", "sp", "1");
                continue;
            }
            token::dec_int_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 4;
                push(frame(code[i].arg1, token::class_int_arrary, offset, size_array, 4));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_double_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 4;
                push(frame(code[i].arg1, token::class_int_arrary, offset, size_array, 4));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_char_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 1;
                push(frame(code[i].arg1, token::class_char_arrary, offset, size_array, 1));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_bool_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 1;
                push(frame(code[i].arg1, token::class_bool_arrary, offset, size_array, 1));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            token::dec_func: {
                continue;
            }
            token::call_func:
            token::call_func_end:
            token::func_return:
            token::
            token::
            token::arg_r_int:
            token::arg_r_double:
            token::arg_r_char           //*函数参数调用立即数
            token::arg_r_bool           //*函数参数调用立即数
            token::
            token::arg_int_class      //*函数参数调用
            token::arg_double_class   //*函数参数调用
            token::arg_char_class     //*函数参数调用
            token::arg_bool_class     //*函数参数调用
            token::
            token::class_int     //int
            token::class_double  //double
            token::class_char    //char
            token::class_bool    //bool
            token::
            token::class_int_array     //int array
            token::class_double_array  //double array
            token::class_char_array    //char array
            token::class_bool_array
            token::
            token::class_int_array_unit
            token::class_double_array_unit
            token::class_char_array_unit
            token::class_bool_array_unit
            token::
            token::arrary_unit //TODO 数组运算到底要不要分出类型， 这里暂时不分用这个代替
            token::
            token::key_void
            token::key_int
            token::key_double
            token::key_string
            token::key_char
            token::key_bool
            token::
            token::r_int
            token::r_double
            token::r_char
            token::r_bool
            token::
            token::
            token::bit_and //&
            token::bit_or  //|
            token::bit_not //~
            token::
            token::log_and //&&
            token::log_or  //||
            token::log_not //~
            token::
            token::plus   //+
            token::minus  //-
            token::times  //*
            token::div    ///
            token::mod    //%
            token::
            token::ver        //->
            token::equ        //==
            token::not_equ    //!=
            token::great_equ  //>=
            token::less_equ   //<=
            token::great      //>
            token::less       //<
            token::
            token::
            token::assign     //=
            token::plus_agn   //+=
            token::minus_agn  //-=
            token::times_agn  //*=
            token::div_agn    ///=
            token::mod_agn    //%=
            token::
            token::l_par       //(
            token::r_par       //)
            token::l_mid_par   //[
            token::r_mid_par   //]
            token::l_big_par   //{
            token::r_big_par   //}
            token::
            token::comma   //,
            token::comment //#
            token::end     //;
            token::
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