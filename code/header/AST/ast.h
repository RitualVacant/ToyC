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
            identifier,
            arguments_declaration,
            arguments_type_list,

            compound_statement,
            block_list,
            block,
            mark_statement,
            initializer,
            postfix_operator
    };
    using idx = std::size_t;
    idx const null = 0;

    //-------------------------------------------

    //basic I
    struct node_loction {
        std::size_t line;
        std::size_t column;
    };

    struct ast_node {
        bool is_init = false;
    };

    struct identifier {
        char name[25] = {};
    };

    //-------------------------------------------

    struct declaration_or_definition {
        ast::idx idx_declaration_declarator = ast::null;
        ast::idx idx_initial_declatator_list = ast::null;
        ast::idx idx_compound_statement = ast::null;

    };

    struct declare_funcion {
        ast::idx idx_return_value_type = ast::null;
        ast::idx idx_funcion_arguments = ast::null;
    };

    struct declare_variable {
    };

    struct definition_struct {

    };

    struct definition_function {
        ast::idx idx_return_value_type = ast::null;
        ast::idx idx_funcion_arguments = ast::null;
        ast::idx idx_compound_statement = ast::null;
    };


    //basic II
    //type
    struct type {
        bool is_volatile = false;
        bool is_const = false;
    };

    //expression
    struct expression {
        ast::idx idx_assignment_expression = ast::null;
    };

    struct assignment_expression {
        ast::idx idx_unary_or_binary_expression = ast::null;
        ast::idx idx_unary_expression = ast::null;
        ast::idx idx_next_assignment_expression = ast::null;
        token assignment_type;
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

    struct initializer {
        ast::idx idx_assignment_expression = ast::null;
        ast::idx idx_initializer_list = ast::null;
    };


    //-------------------------------------------

    //constant
    struct constant {
        std::array<char, 50> const_value;
    };

    struct constant_int {
    };
    struct constant_flaot {
    };
    struct constant_string {
    };

    //expression

    struct conditional_expression {
        ast::idx idx_binary_expression = ast::null;
        ast::idx idx_expression = ast::null;
        ast::idx idx_conditional_expression = ast::null;

    };

    struct binary_expression {
        token token_operator = token::invalid;
        ast::idx idx_left_node = ast::null;
        ast::idx idx_right_node = ast::null;
    };

    struct unary_expression {
        bool is_sizeof = false;

        ast::idx idx_declaration_declatator = ast::null;
        token unary_operator = token::invalid;
        ast::idx idx_unary_expression = ast::null;

        ast::idx idx_postfix_expression = ast::null;
    };

    struct postfix_expression {
        ast::idx idx_primary_expression = ast::null;
        ast::idx idx_postfix_operator = ast::null;
    };
    struct postfix_operator {
        token postfix_operator = token::invalid;
        ast::idx idx_expression = ast::null;
        ast::idx idx_identifier = ast::null;
        ast::idx idx_assignment_expression_list = ast::null;
        ast::idx idx_next_postfix_operator = ast::null;
    };

    struct primary_expression {

    };

    //declare
    struct declare_var {

    };

    struct declarator {
        bool is_ptr = false;
        ast::idx idx_direct_declarator = ast::null;
    };


    struct initial_declarator_list {
        ast::idx idx_initial_declarator = ast::null;

    };

    struct initial_declarator {
        ast::idx idx_declarator = ast::null;
        ast::idx idx_next_initial_declarator = ast::null;
        ast::idx idx_initial_value = ast::null;
    };

    #include "declataror.h"

    struct direct_declarator {
        ast::idx idx_identifier = ast::null;
        ast::idx idx_declarator = ast::null;
        ast::idx idx_arguments_type_list = ast::null;
    };

    struct compound_statement {
        ast::idx idx_block_list = ast::null;
    };

    struct array_declarator {

    };

    struct arguments_type_list {
        ast::idx idx_argument_declaration = ast::null;
    };

    //DROP
    struct arguments_list {
        //ast::idx idx_arugments_declaration = ast::null;
        //ast::idx idx_arguments_list_next = ast::null;
        //ast::idx idx_
    };

    struct idnetifier_list {

    };

    struct block_list {
        ast::idx idx_block = ast::null;
    };
    struct block {
        token type_block_by_token = token::invalid;
        ast::idx idx_statement = ast::null;
        ast::idx idx_declaration = ast::null;
      //ast::idx idx_if_statement = ast::null;
      //ast::idx idx_while_statement = ast::null;
      //ast::idx idx_do_while_statement = ast::null;
      //ast::idx idx_swtich_statement = ast::null;
      //ast::idx idx_for_statement = ast::null;
      //ast::idx idx_next_block = ast::null;
    };

    struct arguments_declaration {
        ast::idx idx_declararion_declarator = ast::null;
        ast::idx idx_declarator = ast::null;
        ast::idx idx_next_arguments_declatation = ast::null;
    };

    struct mark_statement {
        ast::idx idx_mark = ast::null;
        ast::idx idx_statement = ast::null;
        ast::idx idx_constant_expression = ast::null;
        ast::idx idx_identifier = ast::null;
        bool is_default = false;
        bool is_case = false;
        bool is_identif = false;
    };

    struct mark {

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
        ast::arguments_declaration arguments_declaration;
        ast::arguments_type_list arguments_type_list;
        ast::compound_statement compound_statement;
        ast::block block;
        ast::block_list block_list;
        ast::mark_statement mark_statement;
        ast::mark mark;
        ast::initializer initializer;
        ast::postfix_operator postfix_operator;
    };

    struct node {
        ast::value value;
        ast::node_type type;
        ast::node_loction node_loction;
        ast::idx next;
    };
}



#endif