#ifndef SIGN_MAP_CPP
#define SIGN_MAP_CPP

#pragma once
#include "./header/class/sign_map.h"
#include "./header/AST/token.h"
#include "worng.cpp"

//
//把函数插入函数表,并把now_arge替换成这个函数的参数表
//
void sign_map::insert_func(func func_) {
    argu_table = func_.argu;
    func_table.push_back(func_);
}


//
//压入一个临时变量
//
void sign_map::push_token(std::tuple<token, std::string> x) {
    //invadlid
    if (std::get<0>(x) == token::invalid) {
        sign_stack.push_back(frame(token::invalid, "", 0));
        return;
    }
    //检查是否已经定义
    for (int i = 0; i < sp; ++i) {
        if (sign_stack[i].name == std::get<1>(x)) {
            worng::redefine(line, colume, x);
        }
    }
    std::size_t size = size_class.at(std::get<0>(x));
    //
    sign_stack.push_back(frame(std::get<0>(x), std::get<1>(x), sp));
    //sp
    sp += size;
    asm_tem_var_push(size);
    return;
}

//
//
//
void sign_map::push_global_sign(std::string &x) {
    if (global_sign.find(x) != global_sign.end()) {
        worng::redefine(line, colume, {token::indentif, x});
    }
    global_sign.insert(x);
    //if (global_sign.find(x) == global_sign.end()) {
    //    worng::print_worng_redef();
    //}
    //global_sign.insert(x);
    return;
}

//
//塞入底部哨兵
//
void sign_map::push_func() {
    push_token(std::tuple(token::invalid, ""));
    return;
}

//
//把这个函数的所有东西出栈，包括参数
//
void sign_map::pop_func() {
    //TODO:这里函数入栈的标志应该是
    while (sign_stack.back().Token != token::invalid) {
        sign_stack.pop_back();
    }
    return;
}

//
//在栈中寻找
//
bool sign_map::find(std::tuple<token, std::string> &x) {
    for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token != token::invalid; --i) {
        if (sign_stack[i].name == std::get<1>(x)) return true;
    }
    return false;
}
//
//在栈中寻找
//
bool sign_map::find(std::string &x) {
    for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token != token::invalid; --i) {
        if (sign_stack[i].name == x) return true;
    }
    return false;
}

//
//返回在栈中的偏移量
//
std::size_t sign_map::at(std::tuple<token, std::string>& x) {
    std::size_t re;
    for (int i = sign_stack.size() - 1; i > 0 && sign_stack[i].Token != token::invalid; --i) {
        if (std::get<1>(x) == sign_stack[i].name) return sign_stack[i].offset;
    }
    fmt::print("\nsign_map::at(std::size_t x)  偏移量的函数有异常\n");
    exit(0);
    return 0;
}
//
//返回在栈中的偏移量
//
std::size_t sign_map::at(std::string &x) {
    std::size_t re;
    for (int i = sign_stack.size() - 1; i > 0 && sign_stack[i].Token != token::invalid; --i) {
        if (x == sign_stack[i].name) return sign_stack[i].offset;
    }
    fmt::print("\nsign_map::at(std::size_t x)  偏移量的函数有异常\n");
    exit(0);
    return 0;
}

//
//在函数参数表中寻找
//
bool sign_map::find_argu(std::string name) {
    for (int i = 0; i < argu_table.size(); ++i) {
        if (std::get<1>(argu_table[i]) == name) return true;
    }
    return false;
}

//
//在函数表中寻找函数
//
bool sign_map::find_func(std::string &x) {
    for (int i = 0; i < func_table.size(); ++i) {
        if (func_table[i].name == x) return true;
    }
    return false;
}

//
//返回变量，或者是函数参数在汇编中的字符串表达
//
std::string sign_map::find_local(std::string &name) {
    if (find(name)) {
        return fmt::format("[rbp - {}]", at(name));
    }
    std::size_t num;
    if (find_argu(name)) {
        for (std::size_t i = 0; i < argu_table.size(); ++i) {
            if (name == std::get<1>(argu_table[i])) {
                num = i;
                break;
            }
        }
        return register_name.at(num);
    }
    return "";
}

//
//返回函数参数，或者函数临时变量的类型
//
token sign_map::find_var_class(std::string name) {
    for (std::size_t i = 0; i < argu_table.size(); ++i) {
        if (name == std::get<1>(argu_table[i]))
            return std::get<0>(argu_table[i]);
    }
    for (std::size_t i = sign_stack.size() - 1; sign_stack[i].Token != token::invalid && i >= 0; --i) {
        if (name == sign_stack[i].name)
            return sign_stack[i].Token;
    }
    return token::invalid;
}

//-----
//asm
//----
void sign_map::asm_init() {
    file << fmt::format("section .text\n")
         << fmt::format("global main\n");
    return;
}

//
//临时变量入栈,只是栈指针下移动，减少，没有赋值
//
void sign_map::asm_tem_var_push(std::size_t size) {
    file << fmt::format("   sub  rsp {}\n", size);
    return;
}

void sign_map::asm_func_head(std::string func_name) {
    file << fmt::format("{}:\n", func_name)
         << fmt::format("   push rbp\n")
         << fmt::format("   mov  rsp rbp\n");
    return;
}

void sign_map::asm_func_end() {
    file << fmt::format("   mov  rbp rsp\n")
         << fmt::format("   pop  rbp\n")
         << fmt::format("   ret\n");
    return;
}

std::string sign_map::asm_ptr_size(std::size_t size) {
    switch (size) {
        case 1: return "byte ptr";
        case 2: return "word ptr";
        case 4: return "dword ptr";
        case 8: return "qword ptr";
        case 10: return "tword";
    default:
        fmt::print("\n指令ptr输入错误\n");
        exit(0);
    }
}
//
//
//
void sign_map::asm_any_auto___r(token which, std::size_t size, std::string source, std::string option) {
    switch (which) {
    case token::plus:
        asm_add_auto___r(size, source, option);
        return;
    case token::minus:
        asm_sub_auto___r(size, source, option);
        return;
    case token::times:
    case token::div:
    case token::mod:
    default:
        break;
    }
}

void sign_map::asm_any_auto_r__(token which, std::size_t size, std::string source, std::string option) {

}

//mov
void sign_map::asm_mov(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   mov  {} {} {}\n", asm_ptr_size(size), source, option);
    return;
}
void sign_map::asm_mov(std::string source, std::string option) {
    file << fmt::format("   mov  {} {}\n", source, option);
    return;
}
void sign_map::asm_mov_auto_r__(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   mov  {}{} {}\n", asm_register_pre(size), source, option);
    return;
}
void sign_map::asm_mov_auto___r(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   mov  {} {}{}\n", source, asm_register_pre(size), option);
    return;
}

//add
void sign_map::asm_add(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   add  {} {} {}\n", asm_ptr_size(size), source, option);
    return;
}
void sign_map::asm_add_auto_r__(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   add  {}{} {}\n", asm_register_pre(size), source, option);
    return;
}
void sign_map::asm_add_auto___r(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   add  {} {}{}\n", source, asm_register_pre(size), option);
    return;
}

//sub
void sign_map::asm_sub(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   sub  {} {} {}\n", asm_ptr_size(size), source, option);
    return;
}
void sign_map::asm_sub_auto_r__(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   sub  {}{} {}\n", asm_register_pre(size), source, option);
    return;
}
void sign_map::asm_sub_auto___r(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   sub  {} {}{}\n", source, asm_register_pre(size), option);
    return;
}

void sign_map::asm_mul(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   mul  {} {} {}\n", asm_ptr_size(size), source, option);
    return;
}

void sign_map::asm_div(std::size_t size, std::string source, std::string option) {
    file << fmt::format("   div  {} {} {}\n", asm_ptr_size(size), source, option);
    return;
}

char sign_map::asm_register_pre(std::size_t size) {
    switch (size) {
        case 1: return '!';
        case 2: return '!';
        case 4: return 'e';
        case 8: return 'r';
    default:
        return '!';
    }
}

#endif