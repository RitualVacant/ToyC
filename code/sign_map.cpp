#ifndef SIGN_MAP_H
#define SIGN_MAP_H
#include "./header/class/sign_map.h"
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "./header/AST/token.h"
#endif

#ifndef WORNG
#define WORNG
#include "worng.cpp"
#endif

void sign_map::push_token(std::tuple<token, std::string> x) {
    for (int i = bp; i < sp; ++i) {
        if (sign_stack[i] == x) {
            worng::print_worng_redef();
        }
    }
    sign_stack.push_back(x);
    return;
}

void sign_map::push_global_sign(std::tuple<token, std::string> x) {
    if (global_sign.find(std::get<1>(x)) == 0) {
        worng::print_worng_redef();
    }
    global_sign.insert(std::get<1>(x));
    //if (global_sign.find(x) == global_sign.end()) {
    //    worng::print_worng_redef();
    //}
    //global_sign.insert(x);
    return;
}

void sign_map::pop_func(std::tuple<token, std::string> x) {
    push_token({token::invalid, ""});
    return;
}

void sign_map::push_func(std::tuple<token, std::string> x) {
    while (std::get<0>(sign_stack.back()) != token::invalid) {
        sign_stack.pop_back();
    }
    //pop invalid
    sign_stack.pop_back();
    return;
}