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
}

//寄存器号数
std::vector<std::string> register_name {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void asm_file::get_code(std::vector<statement> &code_, std::vector<func> &func_table_) {
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

std::string asm_file::find_var_location(std::string name) {
    for (std::size_t i = 0; i < symbol_stack.size(); ++i) {
        if (name == symbol_stack[i].name)
            return symbol_stack[i].location;
    }
    fmt::print("\nan unknow error at asm_file::find_var_location()\n");
    exit(0);
}

void asm_file::asm_write_code(std::string symbol, std::string arg1, std::string arg2) {
    file << fmt::format("   {:8}{:8}{:8}\n", symbol, arg1, arg2);
    return;
}

void asm_file::asm_write_lable(std::string lable) {
    file << fmt::format("{}:\n", lable);
    return;
}

void asm_file::asm_write_mov(std::size_t size, std::string arg1, std::string arg2) {
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
            case token::invalid: {
                fmt::print("\na unknow error at asm_file::read_code()\n");
                exit(0);
            }
            case token::jmp: {
                asm_write_code("jmp", code[i].arg1, "");
                continue;
            }
            case token::lable: {
                asm_write_lable(code[i].arg1);
                continue;
            }
            case token::dec_int: {
                push(stack_frame(code[i].arg1, token::class_int, sp + 4, 4, 4));
                asm_write_code("sub", "sp", "4");
                continue;
            }
            case token::dec_double: {
                push(stack_frame(code[i].arg1, token::class_double, sp + 4, 4, 4));
                asm_write_code("sub", "sp", "4");
                continue;
            }
            case token::dec_char: {
                push(stack_frame(code[i].arg1, token::class_char, sp + 1, 1, 1));
                asm_write_code("sub", "sp", "1");
                continue;
            }
            case token::dec_bool: {
                push(stack_frame(code[i].arg1, token::class_bool, sp + 1, 1, 1));
                asm_write_code("sub", "sp", "1");
                continue;
            }
            case token::dec_int_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 4;
                push(stack_frame(code[i].arg1, token::class_int_array, sp + size_array, size_array, 4));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_double_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 4;
                push(stack_frame(code[i].arg1, token::class_double_array, sp + size_array, size_array, 4));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_char_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 1;
                push(stack_frame(code[i].arg1, token::class_char_array, sp + size_array, size_array, 1));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_bool_arrary: {
                std::size_t size_array = std::stoul(code[i].arg1) * 1;
                push(stack_frame(code[i].arg1, token::class_bool_array, sp + size_array, size_array, 1));
                asm_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_func: {
                asm_write_lable(code[i].arg1);
                asm_write_code("push", "rbp", "");
                asm_write_code("mov", "rbp", "rsp");
                continue;
            }
            case token::call_func: {
                std::string name_func = code[i].arg1;
                func &now_func = find_func(name_func);
                for (int j = 0; code[i].symbol != token::call_func_end; ++i, ++j) {
                    switch (code[i].symbol) {
                        case token::arg_r_int: {
                            asm_write_mov(4, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_r_double: {
                            asm_write_mov(4, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_r_char: {       //*函数参数调用立即数
                            asm_write_mov(1, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_r_bool: {        //*函数参数调用立即数
                            asm_write_mov(1, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_int_class: {   //*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_write_mov(4, now_func.argu[j].location, var_location);
                            continue;
                        }
                        case token::arg_double_class: {//*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_write_mov(4, now_func.argu[j].location, var_location);
                            continue;
                        }
                        case token::arg_char_class: {  //*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_write_mov(1, now_func.argu[j].location, var_location);
                            continue;
                        }
                        case token::arg_bool_class: {  //*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_write_mov(1, now_func.argu[j].location, var_location);
                            continue;
                        }
                        default: {
                            fmt::print("\nan unknow error at asm_file::read_code()\n");
                            exit(0);
                        }
                    }
                }
            }
            case token::call_func_end: {
                asm_write_code("call", code[i].arg1, "");
            }
            case token::func_return: {
                symbol_stack.clear();
                asm_write_code("mov", "rsp", "rbp");
                asm_write_code("pop", "rbp", "");
                asm_write_code("ret", "", "");
            }

          //case token::class_int:    //int
          //case token::class_double: //double
          //case token::class_char:   //char
          //case token::class_bool:   //bool
            /*
            case token::class_int_array     //int array
            case token::class_double_array  //double array
            case token::class_char_array    //char array
            case token::class_bool_array
            
            case token::class_int_array_unit
            case token::class_double_array_unit
            case token::class_char_array_unit
            case token::class_bool_array_unit
            
            case token::arrary_unit //TODO 数组运算到底要不要分出类型， 这里暂时不分用这个代替
            case token::
            case token::key_void
            case token::key_int
            case token::key_double
            case token::key_string
            case token::key_char
            case token::key_bool
            
            case token::r_int
            case token::r_double
            case token::r_char
            case token::r_bool
            
            case token::bit_and //&
            case token::bit_or  //|
            case token::bit_not //~
            
            case token::log_and //&&
            case token::log_or  //||
            case token::log_not //~
            
            case token::plus   //+
            case token::minus  //-
            case token::times  //*
            case token::div    ///
            case token::mod    //%
            
            case token::ver        //->
            case token::equ        //==
            case token::not_equ    //!=
            case token::great_equ  //>=
            case token::less_equ   //<=
            case token::great      //>
            case token::less       //<
            
            case token::assign     //=
            case token::plus_agn   //+=
            case token::minus_agn  //-=
            case token::times_agn  //*=
            case token::div_agn    ///=
            case token::mod_agn    //%=
            
            case token::l_par       //(
            case token::r_par       //)
            case token::l_mid_par   //[
            case token::r_mid_par   //]
            case token::l_big_par   //{
            case token::r_big_par   //}
            
            case token::comma   //,
            case token::comment //#
            case token::end     //;
            
            case token::indentif  //标识符
            case token::key_if         //if
            case token::key_else       //else
            case token::key_true       //true
            case token::key_false      //false
            case token::key_while      //while
            case token::key_for        //for
            case token::key_null       //null
            case token::key_func       //func
            case token::key_return     //return
            case token::key_break      //break
            case token::key_continue   //continue
            */
            default: {
                fmt::print("\nan unknow error at asm::read_code()\n");
                exit(0);
            }
        }
    }
}

#endif