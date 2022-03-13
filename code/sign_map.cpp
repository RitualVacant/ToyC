#ifndef SIGN_MAP_CPP
#define SIGN_MAP_CPP

#pragma once
#include "./header/class/sign_map.h"
#include "./header/AST/token.h"
#include "worng.cpp"

void sign_map::set_info(std::size_t line_, std::size_t colume_, token now_token_) {
    line = line_;
    colume = colume_;
    now_token = now_token_;
    return;
}

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
    //TODO这里还有赋值入栈的初始化
    //asm_tem_var_push(size, "324");
    asm_tem_var_push(size, "");
    return;
}

void sign_map::push_global_sign(std::string x) {
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

void sign_map::push_func(std::tuple<token, std::string> x) {
    push_token(std::tuple(token::invalid, ""));
    //查看函数是否定义
    if (!find_func(std::get<1>(x))) {
        func_set.insert(std::get<1>(x));
        //函数头
        asm_func_head(std::get<1>(x));
    }
    else worng::redefine(line, colume, x);
    return;
}

void sign_map::pop_func() {
    //TODO:这里函数入栈的标志应该是
    while (sign_stack.back().Token != token::invalid) {
        sign_stack.pop_back();
    }
    return;
}

bool sign_map::find(std::tuple<token, std::string> x) {
    for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token != token::invalid; --i) {
        if (sign_stack[i].name == std::get<1>(x)) return true;
    }
    return false;
}

bool sign_map::find_func(std::string x) {
    return !(func_set.find(x) == func_set.end());
}

//-----
//asm
//----

void sign_map::asm_tem_var_push(std::size_t size, std::string value = "") {
    std::string push;
    switch (size) {
        case 1:
            push = "pushb";
            break;
        case 4:
            push = "pushw";
            break;
        case 8:
            push = "pushl";
            break;
    }
    file << fmt::format("{} {}\n", push, value);
}

void sign_map::asm_func_head(std::string func_name) {
    file << fmt::format("{}:\n", func_name)
         << fmt::format("   pushl %ebp\n")
         << fmt::format("   movl %esp %ebp\n");
    return;
}

void sign_map::asm_func_end() {
    file << fmt::format("   movl %ebp %esp\n")
         << fmt::format("   popl %ebp\n")
         << fmt::format("   ret\n");
    return;
}

#endif