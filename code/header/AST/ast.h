#ifndef AST_H
#define AST_H

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "token.h"
//#include <variant>
//#include <string_view>
#include <array>

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
            definition_struct,
            constant,
            operators,

            initial_declarator,


            //which class??
            direct_declarator,
            declarator,
            declaration_declarator,
            initial_declarator_list,
            declaration_or_definition,
            identifier
    };
    using ptr = std::size_t;
    ptr const null = 0;

    //-------------------------------------------

    //basic I
    struct node_loction {
        std::size_t line;
        std::size_t column;
    };

    struct ast_node {
        node_loction loc;
    };

    struct declare : ast_node {
        std::array<char, 25> name;
    };

    struct identifier {
        std::array<char, 25> name;
    };

    //-------------------------------------------

    struct declaration_or_definition : declare {
        ast::ptr ptr_declaration_declarator = ast::null;
        ast::ptr ptr_initial_declatator_list = ast::null;
    };

    struct declare_funcion : declare {
        ast::ptr ptr_return_value_type;
        ast::ptr ptr_funcion_arguments;
    };

    struct declare_variable : ast_node {
    };

    struct definition_struct : ast_node {

    };

    struct definition_function : ast_node {
        ast::ptr ptr_return_value_type;
        ast::ptr ptr_funcion_arguments;
        ast::ptr ptr_compound_statement;
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

    struct declarator {
        bool is_ptr = false;
        ast::ptr ptr_direct_declarator;
    };


    struct initial_declarator_list {
        ast::ptr ptr_initial_declarator;

    };

    struct initial_declarator : ast_node {
        ast::ptr ptr_declarator = ast::null;
        ast::ptr ptr_next_initial_declarator = ast::null;
        ast::ptr ptr_initial_value = ast::null;
    };

    #include "declataror.h"

    struct direct_declarator {
        ast::ptr ptr_identifier = ast::null;
        ast::ptr ptr_declarator = ast::null;
    };

    struct array_declarator {

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
        ast::declaration_declarator declaration_declarator;
        ast::declarator declarator;
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
        ast::initial_declarator initial_declarator;
        ast::initial_declarator_list initial_declarator_list;
        ast::declaration_or_definition declaration_or_definition;
        ast::direct_declarator direct_declarator;
        ast::identifier identifier;
    };

    struct node {
        ast::value value;
        ast::node_type type;
        ast::ptr loc;
        ast::ptr next;
    };
}



#endif