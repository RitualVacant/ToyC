#ifndef VALUE_CPP
#define VALUE_CPP

#pragma once
#include <any>
#include <unordered_map>
#include <string>
#include <vector>
#include "ast.h"

enum class value_type : char {
    int_type,
    double_type,
    string_type,
    bool_type,
    char_type,
    null_type,
    array_type,
};

/*
namespace script {

    struct block {
        explicit block() = default;
        ~block() = default;
        std::vector<ast::statement*> statement;
    };

    struct value {
        value_type type;
        std::any data;

        explicit value() {};
        explicit value(value_type type_) : type(type_) {};
        explicit value(value_type type_, std::any data_) : data(data_) {};
    };

    struct function {
        block* block;
        std::string name;

        explicit function() = default;
        ~function() {delete block;}
    };


    struct variable {
        std::string name;
        value value;

        explicit variable() = default;
        ~variable() = default;
    };
}


class context {
    private:
        std::unordered_map<std::string, script::variable*> all_var;
        std::unordered_map<std::string, script::function*> all_func;

    public:
        explicit context() = default;
        ~context() = default;

        void create_var(std::string &name, script::value type);
        void create_func(std::string &name, script::function* func);

        bool has_func(std::string &name);
        bool has_var(std::string &name);

        script::function* get_function(std::string &name);
        script::variable* get_variable(std::string &name);
};

void context::create_var(std::string &name, script::value type) {
    auto* var = new script::variable;
    var->name = name;
    var->value = type;
    //可以重复命名
    all_var[name] = var;
};

void context::create_func(std::string &name, script::function* func) {
    all_func.insert({name, func});
};

bool context::has_func(std::string &name) {
    return all_func.find(name) != all_func.end();
};

bool context::has_var(std::string &name) {
    return all_var.find(name) != all_func.end();
};

script::function* context::get_function(std::string &name) {
    if (auto f = all_func.find(name); f != all_func.end()) return f->second;
    return nullptr;
};

script::variable* context::get_variable(std::string &name) {
    if (auto f = all_var.find(name); f != all_var.end()) return f->second;
    return nullptr;
};

class run_time : public context {
    using bulit_in_func_type = script::value (*)(run_time*, std::);
    private:
        std::unordered_map<token, std::string> m;
        std::vector<ast::statement*> statements;
    public:
        explicit run_time();

        bool fw;
}

*/
#endif