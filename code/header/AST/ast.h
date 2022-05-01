#ifndef AST_H
#define AST_H

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "token.h"
#include "declararor.h"
#include <variant>

namespace ast {
    enum class node_type {
            type,
            expression,
                assignment_expression,
                conditional_expression,
                binary_expression,
                unary_expression,
                postfix_expression,
                primary_expression,
            statement,
            declare_varible,
            declare_function,
            definition_function,
            constant,
            operators
    };
    using ptr = std::size_t;
    ptr const null = 0;

    //-------------------------------------------

    //basic I
    struct node_loction {
        std::size_t line;
        std::size_t column;
    };

    //-------------------------------------------
    struct declare_funcion {
        node_loction loc;
    };
    struct declare_variable {
        node_loction loc;
    };
    struct definition_function {
        node_loction loc;
    };


    //basic II
    //type
    struct type {
        bool is_volatile = false;
        bool is_const = false;
    };

    //expression
    struct expression {
    };

    //statement
    struct statement {
    };

    //declare or definition
    char const var_dec  = 0;
    char const var_def  = 1;
    char const func_def = 2;

    //operator
    struct operators {

    };

    //constant
    struct constant {

    };


    //-------------------------------------------

    //constant
    struct constant_int {
    };
    struct constant_flaot {
    };
    struct constant_string {
    };

    //expression
    struct assignment_expression {

    };

    struct conditional_expression {

    };

    struct binary_expression {

    };

    struct unary_expression {

    };

    struct postfix_expression {

    };

    struct primary_expression {

    };

    //declare
    struct declare_var {

    };

    //DROP
    struct declaration_declarator {

    };

    struct initial_declatator_list {

    };

    struct initial_declatator {

    };

    struct declatator {

    };

    struct direct_declatator {

    };

    struct array_declatator {

    };

    struct arguments_type_list {

    };

    struct idnetifier_list {

    };

    struct arguments_list {

    };

    struct arguments_declaration {

    };


    // statement node

    //break
    struct break_statement {

    };
    //continue
    struct continue_statement {

    };
    //expression
    struct expr_statement {

    };
    //return
    union value {
        ast::node_loction node_loction;
        ast::type type;
        ast::expression expression;
        ast::statement statement;
        ast::declare_funcion declare_funcion;
        ast::declare_variable declare_variable;
        ast::definition_function definition_function;
        ast::operators operators;
        ast::constant constant;
        ast::constant_flaot constant_flaot;
        ast::constant_string constant_string;
        ast::assignment_expression assignment_expression;
        ast::conditional_expression conditional_expression;
        ast::binary_expression binary_expression;
        ast::unary_expression unary_expression;
        ast::postfix_expression postfix_expression;
        ast::primary_expression primary_expression;
        ast::declare_var declare_var;
    };

    struct node {
        ast::value value;
        ast::node_type node_type;
    };
}



#endif