#ifndef SIGN_MAP_CPP
#define SIGN_MAP_CPP

#include "symbol_table.h"
#include "token.h"

symbol_table::symbol_table(std::string &file_path_, bool really_output_asm_code_)
    : input_file_path(file_path_) {
  if (really_output_asm_code_)
    return;

  file.open(input_file_path, std::ofstream::out);
  if (!file.is_open()) {
    fmt::print(fg(fmt::color::red), "\nfail to open the file!\n");
    fmt::print(fg(fmt::color::red), "please check the output file's path\n");
    fmt::print(fg(fmt::color::red), "--end--\n");
    exit(1);
  }
}

symbol_table::~symbol_table() {
  file.close();
}

void symbol_table::insert_func(func func_) {
  func_table.push_back(func_);
}

//压入一个临时变量
void symbol_table::push_var(frame x) {
  symbol_stack.push_back(x);
  return;
}

void symbol_table::clear_symbol_stack() {
  symbol_stack.clear();
}

//返回函数参数，或者函数临时变量的类型
// TODO 要能返回函数参数的类型
token symbol_table::find_var_class(std::string name) {
  for (std::size_t i = 0; i < symbol_stack.size(); ++i) {
    if (name == symbol_stack[i].name)
      return symbol_stack[i].var_class;
  }
  return token::invalid;
}
//
//
//
void symbol_table::push_global_sign(std::string &x) {
  if (global_sign.find(x) != global_sign.end()) {
    // worng::redefine(line, colume, {token::identify, x});
  }
  global_sign.insert(x);
  // if (global_sign.find(x) == global_sign.end()) {
  //     worng::print_worng_redef();
  // }
  // global_sign.insert(x);
  return;
}


void symbol_table::pop_func() {
  // TODO:这里函数入栈的标志应该是
  // while (sign_stack.back().Token != token::invalid) {
  //     sign_stack.pop_back();
  // }
  return;
}

//在栈中寻找
bool symbol_table::find(std::tuple<token, std::string> &x) {
  // for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token
  // != token::invalid; --i) {
  //     if (sign_stack[i].name == std::get<1>(x)) return true;
  // }
  return false;
}

//在栈中寻找
bool symbol_table::find(std::string &x) {
  // for (std::size_t i = sign_stack.size() - 1; i >= 0 && sign_stack[i].Token
  // != token::invalid; --i) {
  //     if (sign_stack[i].name == x) return true;
  // }
  return false;
}

std::size_t symbol_table::at(std::tuple<token, std::string> &x) {
  // std::size_t re;
  // for (int i = sign_stack.size() - 1; i > 0 && sign_stack[i].Token !=
  // token::invalid; --i) {
  //     if (std::get<1>(x) == sign_stack[i].name) return sign_stack[i].offset;
  // }
  fmt::print("\nsymbol_list::at(std::size_t x)  偏移量的函数有异常\n");
  exit(0);
  return 0;
}

std::size_t symbol_table::at(std::string &x) {
  // std::size_t re;
  // for (int i = sign_stack.size() - 1; i > 0 && sign_stack[i].Token !=
  // token::invalid; --i) {
  //     if (x == sign_stack[i].name) return sign_stack[i].offset;
  // }
  fmt::print("\nsymbol_list::at(std::size_t x)  偏移量的函数有异常\n");
  exit(0);
  return 0;
}

bool symbol_table::find_argu(std::string name) {
  // for (int i = 0; i < argu_table.size(); ++i) {
  //     if (std::get<1>(argu_table[i]) == name) return true;
  // }
  return false;
}

bool symbol_table::find_func(std::string &x) {
  // for (int i = 0; i < func_table.size(); ++i) {
  //     if (func_table[i].name == x) return true;
  // }
  return false;
}

std::string symbol_table::find_local(std::string &name) {
  if (find(name)) {
    return fmt::format("[rbp - {}]", at(name));
  }
  // std::size_t num;
  if (find_argu(name)) {
    // for (std::size_t i = 0; i < argu_table.size(); ++i) {
    //     if (name == std::get<1>(argu_table[i])) {
    //         num = i;
    //         break;
    //     }
    // }
    // return register_name.at(num);
  }
  return "";
}

std::string symbol_table::find_func_return_var_value(std::string func_name) {
  for (std::size_t i = 0; i < func_table.size(); ++i) {
    if (func_name == func_table[i].name)
      return func_table[i].name_return;
  }

  fmt::print("can't find func in func_table\n");
  fmt::print("at symbol_table::fund_func_return_var_value()\n");
  exit(0);
  return "";
}

void symbol_table::init_loc_argu() {
  for (std::size_t i = 0; i < func_table.size(); i++) {
    //在寄存器中的传参
    for (std::size_t j = 0; j < 6 && j < func_table[i].argu.size(); ++j) {
      func_table[i].argu[j].location = argu_register_loc[j];
    }
    //在栈中的传参
    for (std::size_t j = 0; j < func_table[i].argu.size(); ++j) {
      // TODO
      // func_table[i].argu[j] =
      switch (func_table[i].argu[j].arg_class) {
        case token::key_int: {
          // TODO
          func_table[i].argu[j].location = fmt::format("[bp-{}]", sp);
          sp += 4;
          break;
        }
        case token::key_char: {
          func_table[i].argu[j].location = fmt::format("[bp-{}]", sp);
          sp += 4;
          break;
        }
        case token::key_double: {
          func_table[i].argu[j].location = fmt::format("[bp-{}]", sp);
          sp += 4;
          break;
        }
        default:
          break;
      }
    }
  }
  return;
}

#endif