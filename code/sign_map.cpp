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
    func_table.push_back(func_);
}


//
//压入一个临时变量
//
void sign_map::push_var(frame x) {
    symbol_stack.push_back(x);
    return;
}


void sign_map::clear_symbol_stack() {
    symbol_stack.clear();
}

//
//返回函数参数，或者函数临时变量的类型
//TODO 要能返回函数参数的类型

token sign_map::find_var_class(std::string name) {
    for (int i = 0; i < symbol_stack.size(); ++i) {
        if (name == symbol_stack[i].name)
            return symbol_stack[i].var_class;
    }
    return token::invalid;
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
//把这个函数的所有东西出栈，包括参数
//
void sign_map::pop_func() {
    //TODO:这里函数入栈的标志应该是
  //while (sign_stack.back().Token != token::invalid) {
  //    sign_stack.pop_back();
  //}
    return;
}

//
//在栈中寻找
//
bool sign_map::find(std::tuple<token, std::string> &x) {
  //for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token != token::invalid; --i) {
  //    if (sign_stack[i].name == std::get<1>(x)) return true;
  //}
    return false;
}
//
//在栈中寻找
//
bool sign_map::find(std::string &x) {
  //for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token != token::invalid; --i) {
  //    if (sign_stack[i].name == x) return true;
  //}
    return false;
}

//
//返回在栈中的偏移量
//
std::size_t sign_map::at(std::tuple<token, std::string>& x) {
    std::size_t re;
  //for (int i = sign_stack.size() - 1; i > 0 && sign_stack[i].Token != token::invalid; --i) {
  //    if (std::get<1>(x) == sign_stack[i].name) return sign_stack[i].offset;
  //}
    fmt::print("\nsign_map::at(std::size_t x)  偏移量的函数有异常\n");
    exit(0);
    return 0;
}
//
//返回在栈中的偏移量
//
std::size_t sign_map::at(std::string &x) {
    std::size_t re;
  //for (int i = sign_stack.size() - 1; i > 0 && sign_stack[i].Token != token::invalid; --i) {
  //    if (x == sign_stack[i].name) return sign_stack[i].offset;
  //}
    fmt::print("\nsign_map::at(std::size_t x)  偏移量的函数有异常\n");
    exit(0);
    return 0;
}

//
//在函数参数表中寻找
//
bool sign_map::find_argu(std::string name) {
  //for (int i = 0; i < argu_table.size(); ++i) {
  //    if (std::get<1>(argu_table[i]) == name) return true;
  //}
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
      //for (std::size_t i = 0; i < argu_table.size(); ++i) {
      //    if (name == std::get<1>(argu_table[i])) {
      //        num = i;
      //        break;
      //    }
      //}
        return register_name.at(num);
    }
    return "";
}

std::string sign_map::find_func_return_var_value(std::string func_name) {
    for (int i = 0; i < func_table.size(); ++i) {
        if (func_name == func_table[i].name)
            return func_table[i].name_return;
    }

    fmt::print("can't find func in func_table\n");
    fmt::print("at sign_map::fund_func_return_var_value()\n");
    exit(0);
    return "";
}
#endif