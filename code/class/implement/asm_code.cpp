#ifndef ASM_FILE_CPP
#define ASM_FILE_CPP

#pragma once
/*
#include "asm_code.h"
#include <sstream>

std::size_t asm_code::sizeof_all_var_in_func(std::size_t now_loc) {
    std::size_t sizeof_all_var = 0;
    for (int i = now_loc; code[i].symbol != token::func_return; ++i) {
        switch (code[i].symbol) {
            case token::dec_int: {
                sizeof_all_var += 4;
                break;
            }
            case token::dec_double: {
                sizeof_all_var += 4;
                break;
            }
            case token::dec_bool: {
                sizeof_all_var += 1;
                break;
            }
            case token::dec_char: {
                sizeof_all_var += 1;
                break;
            }
            //TODO 运行时报错
            case token::dec_int_arrary: {
                sizeof_all_var += std::stoul(code[i].arg2) * 4;
                break;
            }
            case token::dec_double_arrary: {
                sizeof_all_var += std::stoul(code[i].arg2) * 4;
                break;
            }
            case token::dec_char_arrary: {
                sizeof_all_var += std::stoul(code[i].arg2) * 1;
                break;
            }
            case token::dec_bool_arrary: {
                sizeof_all_var += std::stoul(code[i].arg2) * 1;
                break;
            }
            default: {
                exit(0);
            }
        }
    }
    return sizeof_all_var;
}

func& asm_code::find_func(std::string name) {
    for (auto &i : table->func_table) {
        if (i.name == name)
            return i;
    }
    fmt::print("can't find function int func_table in asm_code::find_func");
    exit(0);
}

std::string asm_code::trans_to_loc(std::string name) {
    if (name.size() == 0) return "";
    if (name[0] == '$') {
        auto res = std::string(name.begin() + 1, name.end());
        return res;
    }
    //TODO
    if (name[0] == '#') return std::string(name.begin() + 1, name.end());
    if (name[0] == '&') {
        int name_end;
        for (name_end = 1; name[name_end] != '['; ++name_end);
        std::string name_array(name.begin() + 1, name.begin() + name_end);

        stack_frame array;
        //array location
        for (std::size_t i = 0; i < symbol_stack.size(); ++i) {
            if (name_array == symbol_stack[i].name)
                array = symbol_stack[i];
        }
        //
        return fmt::format("[bp-{}-{}*{}]", array.offset, array.size_unit, trans_to_loc(std::string(name.begin() + name_end + 1, name.end() - 1)));
    }
    for (std::size_t i = 0; i < symbol_stack.size(); ++i) {
        if (name == symbol_stack[i].name)
            return symbol_stack[i].location;
    }
    fmt::print("\n can't find {} location in asm_code::trans_to_loc", name);
    return "";
}

//TODO寄存器号数
//已经弃用
//std::vector<std::string> register_name {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void asm_code::get_code(std::vector<statement> &code_, std::vector<func> &func_table_) {
  //code = std::move(code_);
  ////table.func_table = std::move(func_table_);

  ////offset of argu in stack
  //std::size_t offset = 0;
  ////init argu location
  //for (auto &i : table.func_table) {
  //    for (std::size_t j = 0; j < i.argu.size(); ++j) {
  //        if (j < 6) {
  //            //i.argu[j].location = register_name[j];
  //        }
  //        //TODO
  //        else {
  //            switch (i.argu[j].arg_class) {
  //            case token::class_int: {
  //            }
  //            case token::class_double:
  //            case token::class_char:
  //            case token::class_bool:
  //            break;
  //            default:
  //                fmt::print("\na unknow error at asm_code::get_code\n");
  //                break;
  //            }
  //        }
  //    }
  //}
    return;
}

void asm_code::push(stack_frame x) {
    sp += x.size_total;
    x.offset = sp;
    symbol_stack.push_back(x);
    return;
}

std::string asm_code::find_var_location(std::string name) {
    for (std::size_t i = 0; i < symbol_stack.size(); ++i) {
        if (name == symbol_stack[i].name)
            return symbol_stack[i].location;
    }
    fmt::print("\nan unknow error at asm_code::find_var_location()\n");
    exit(0);
}

void asm_code::asm_code_write_code(std::string symbol, std::string arg1, std::string arg2) {
    if (arg2.size() == 0) {
        if (arg1.size() == 0) file << fmt::format("    {}\n", symbol);
        else                  file << fmt::format("    {:8}{}\n", symbol, arg1);
    }
    else                      file << fmt::format("    {:8}{:8}{}\n", symbol, arg1 + ',', arg2);
    //file << fmt::format("   {:8}{:8}{:8}\n", symbol, arg1, arg2);
    //std::cout << fmt::format("   {:8}{:8}{:8}\n", symbol, arg1, arg2);
    return;
}

void asm_code::asm_code_write_lable(std::string lable) {
    file << fmt::format("{}:\n", lable);
    return;
}

void asm_code::asm_code_write_mov(std::size_t size, std::string arg1, std::string arg2) {
    switch (size) {
        case 1:
            file << fmt::format("    {:8}{:12}{:8}{}\n", "mov", "byte ptr", arg1 + ',', arg2);
            return;
        case 2:
            file << fmt::format("    {:8}{:12}{:8}{}\n", "mov", "word ptr", arg1 + ',', arg2);
            return;
        case 4:
            file << fmt::format("    {:8}{:12}{:8}{}\n", "mov", "dword ptr", arg1 + ',', arg2);
            return;
        case 8:
            file << fmt::format("    {:8}{:12}{:8}{}\n", "mov", "qword ptr", arg1 + ',', arg2);
            return;
        default:
            fmt::print("\na unknow error at asm_code::asm_code_write_mov()\n");
            exit(0);
    }
    return;
}

void asm_code::read_code() {
    for (std::size_t i = 0; i < code.size(); ++i) {
        switch (code[i].symbol) {
            case token::invalid: {
                fmt::print("\na unknow error at asm_code::read_code()\n");
                exit(0);
            }
            case token::jmp: {
                asm_code_write_code("jmp", code[i].arg1, "");
                continue;
            }
            case token::lable: {
                asm_code_write_lable(code[i].arg1);
                continue;
            }
            case token::dec_int: {
                push(stack_frame(code[i].arg1, token::class_int, sp + 4, 4, 4));
                asm_code_write_code("sub", "sp", "4");
                continue;
            }
            case token::dec_double: {
                push(stack_frame(code[i].arg1, token::class_double, sp + 4, 4, 4));
                asm_code_write_code("sub", "sp", "4");
                continue;
            }
            case token::dec_char: {
                push(stack_frame(code[i].arg1, token::class_char, sp + 1, 1, 1));
                asm_code_write_code("sub", "sp", "1");
                continue;
            }
            case token::dec_bool: {
                push(stack_frame(code[i].arg1, token::class_bool, sp + 1, 1, 1));
                asm_code_write_code("sub", "sp", "1");
                continue;
            }
            case token::dec_int_arrary: {
                std::size_t size_array = std::stoul(code[i].arg2) * 4;
                push(stack_frame(code[i].arg1, token::class_int_array, sp + size_array, size_array, 4));
                asm_code_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_double_arrary: {
                std::size_t size_array = std::stoul(code[i].arg2) * 4;
                push(stack_frame(code[i].arg1, token::class_double_array, sp + size_array, size_array, 4));
                asm_code_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_char_arrary: {
                std::size_t size_array = std::stoul(code[i].arg2) * 1;
                push(stack_frame(code[i].arg1, token::class_char_array, sp + size_array, size_array, 1));
                asm_code_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_bool_arrary: {
                std::size_t size_array = std::stoul(code[i].arg2) * 1;
                push(stack_frame(code[i].arg1, token::class_bool_array, sp + size_array, size_array, 1));
                asm_code_write_code("sub", "sp", std::to_string(size_array));
                continue;
            }
            case token::dec_func: {
                asm_code_write_lable(code[i].arg1);
                asm_code_write_code("push", "rbp", "");
                asm_code_write_code("mov", "rbp", "rsp");
                //一次性分配完
                std::size_t size = sizeof_all_var_in_func(i);
                asm_code_write_code("sub", "rsp", std::to_string(size));
                continue;
            }
            case token::call_func: {
                std::string name_func = code[i].arg1;
                func &now_func = find_func(name_func);
                ++i;
                for (int j = 0; code[i].symbol != token::call_func_end; ++i, ++j) {
                    switch (code[i].symbol) {
                        case token::arg_r_int: {
                            asm_code_write_mov(4, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_r_double: {
                            asm_code_write_mov(4, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_r_char: {       //*函数参数调用立即数
                            asm_code_write_mov(1, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_r_bool: {        //*函数参数调用立即数
                            asm_code_write_mov(1, now_func.argu[j].location, code[i].arg1);
                            continue;
                        }
                        case token::arg_int_class: {   //*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_code_write_mov(4, now_func.argu[j].location, var_location);
                            continue;
                        }
                        case token::arg_double_class: {//*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_code_write_mov(4, now_func.argu[j].location, var_location);
                            continue;
                        }
                        case token::arg_char_class: {  //*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_code_write_mov(1, now_func.argu[j].location, var_location);
                            continue;
                        }
                        case token::arg_bool_class: {  //*函数参数调用
                            std::string var_location = find_var_location(code[i].arg1);
                            asm_code_write_mov(1, now_func.argu[j].location, var_location);
                            continue;
                        }
                        default: {
                            fmt::print("\nan unknow error at asm_code::read_code()\n");
                            fmt::print("case token::call_func:\n");
                            fmt::print("token: {}\n", token_to_string.at(code[i].symbol));
                            exit(0);
                        }
                    }
                }
                continue;
            }
            case token::call_func_end: {
                asm_code_write_code("call", code[i].arg1, "");
                continue;
            }
            case token::func_return: {
                symbol_stack.clear();
                asm_code_write_code("mov", "rsp", "rbp");
                asm_code_write_code("pop", "rbp", "");
                asm_code_write_code("ret", "", "");
                continue;
            }
            case token::assign: {   //=
                //TODO 没有指定大小
                asm_code_write_mov(4, trans_to_loc(code[i].result), trans_to_loc(code[i].arg1));
                continue;
            }

          //case token::class_int:    //int
          //case token::class_double: //double
          //case token::class_char:   //char
          //case token::class_bool:   //bool

            case token::class_int_array:    //int array
            case token::class_double_array: //double array
            case token::class_char_array:   //char array
            case token::class_bool_array:

            case token::class_int_array_unit:
            case token::class_double_array_unit:
            case token::class_char_array_unit:
            case token::class_bool_array_unit:

            case token::arrary_unit://TODO 数组运算到底要不要分出类型， 这里暂时不分用这个代替
            case token::key_void:
            case token::key_int:
            case token::key_double:
            case token::key_char:

            case token::r_int:
            case token::r_double:
            case token::r_char:
            case token::r_bool:

            case token::bit_and://&
            case token::bit_or: //|
            case token::bit_not://~

            case token::log_and://&&
            case token::log_or: //||
            case token::log_not://~

            case token::plus: { //+
                continue;
            }
            case token::minus: {//-
                continue;
            }
            case token::times: {//*
                continue;
            }
            case token::div:  { ///
                continue;
            }
            case token::mod: {  //%
                continue;
            }

            //case token::ver        //->
            case token::equ: {       //==
                asm_code_write_code("cmp", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                asm_code_write_code("je", code[i].result, "");
                continue;
            }

            case token::not_equ:{  //!=
                asm_code_write_code("cmp", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                asm_code_write_code("jne", code[i].result, "");
                continue;
            }
            case token::great_equ:  {//>=
                asm_code_write_code("cmp", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                asm_code_write_code("jge", code[i].result, "");
                continue;
            }
            case token::less_equ: {//<=
                asm_code_write_code("cmp", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                asm_code_write_code("jle", code[i].result, "");
                continue;
            }
            case token::great: {   //>
                asm_code_write_code("cmp", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                asm_code_write_code("jg", code[i].result, "");
                continue;
            }
            case token::less: {    //<
                asm_code_write_code("cmp", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                asm_code_write_code("jl", code[i].result, "");
                continue;
            }
            case token::plus_agn: {//+=
                asm_code_write_code("add", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                continue;
            }
            case token::minus_agn: {//-=
                asm_code_write_code("sub", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                continue;
            }
            case token::times_agn: {//*=
                asm_code_write_code("mul", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                continue;
            }
            case token::div_agn: { ///=
                asm_code_write_code("div", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                continue;
            }
            case token::mod_agn: { //%=
                //TODO
                asm_code_write_code("", trans_to_loc(code[i].arg1), trans_to_loc(code[i].arg2));
                continue;
            }

            case token::l_par: {    //(
                continue;
            }
            case token::r_par: {    //)
                continue;
            }
            case token::l_mid_par: {//[
                continue;
            }
            case token::r_mid_par: {//]
                continue;
            }
            case token::l_big_par: {//{
                continue;
            }
            case token::r_big_par: {//}
                continue;
            }

            case token::comma: {//,
                continue;
            }
            case token::comment: {//#
                continue;
            }
            case token::end: {  //;
                continue;
            }
            case token::identif: {//标识符
                continue;
            }
            case token::key_if: {      //if
                continue;
            }
            case token::key_else: {    //else
                continue;
            }
            case token::key_true: {    //true
                continue;
            }
            case token::key_false: {   //false
                continue;
            }
            case token::key_while: {   //while
                continue;
            }
            case token::key_for: {     //for
                continue;
            }
            case token::key_null: {    //null
                continue;
            }
            case token::key_func: {    //func
                continue;
            }
            case token::key_return: {  //return
                continue;
            }
            case token::key_break: {   //break
                continue;
            }
            case token::key_continue: { //continue
                continue;
            }
            default: {
                fmt::print("\nan unknow error at asm_code::read_code()\n");
                fmt::print("at end of read_code()\n");
                fmt::print("token: {}\n", token_to_string.at(code[i].symbol));
                exit(0);
            }
        }
    }
}


bool asm_code::register_is_used(register_name name) {
    return register_used[static_cast<std::size_t>(name)];
}

*/
#endif