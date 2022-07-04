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
    std::size_t const array_in_struct_size = 25;

    enum class declarator_type : unsigned char {
        type_empty,

        type_void,
        type_int,
        type_short_int,
        type_char,
        type_float,
        type_double,
        type_long_int,
        type_long_long_int,
        type_struct,
        type_enum
    };

    enum class declarator_store : unsigned char {
        store_empty,

        store_extern,
        store_static,
        store_typedef
    };

    enum class declarator_limit : unsigned char {
        limit_empty,
        limit_Alignas
    };

    enum class declarator_sign : bool {
        sign_signed,
        sign_unsigned
    };

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
            array_declarator,

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
            postfix_operator,

            case_label,
            default_label,

            if_statement,
            while_statement,
            do_while_statement,
            for_statement,
            switch_statement,
            goto_statement,
            continue_statement,
            return_statement,
            break_statement,

            //second
            function_declaration,
            function_definition,
            enum_definition,
            array_definintion,
            struct_declaration,
            struct_defination,
            basic_type_declaration,

    };
    using idx = std::size_t;
    idx const null{0};

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
        char name[ast::array_in_struct_size] = {};
    };

    //-------------------------------------------

    struct declaration_or_definition {
        ast::idx idx_declaration_declarator = ast::null;
        ast::idx idx_initial_declatator_list = ast::null;
        ast::idx idx_compound_statement = ast::null;
        ast::idx idx_next_declaration_or_definition = ast::null;
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

    struct array_declarator {
        ast::idx idx_next_array_declarator = ast::null;
        ast::idx idx_constant = ast::null;
    };

    //constant

    struct initializer {
        ast::idx idx_assignment_expression = ast::null;
        ast::idx idx_initializer_list = ast::null;
    };


    //-------------------------------------------

    //constant
    struct constant {
        char const_value[array_in_struct_size] = {};
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

        token unary_operator = token::invalid;
        ast::idx idx_declaration_declatator = ast::null;
        ast::idx idx_unary_expression = ast::null;
        ast::idx idx_postfix_expression = ast::null;

    };

    struct postfix_expression {
        ast::idx idx_primary_expression = ast::null;
        ast::idx idx_postfix_operator = ast::null;
    };
    struct postfix_operator {
        token postfix_operator = token::invalid;
        ast::idx idx_array_idx_assignment_expression = ast::null;
        ast::idx idx_identifier = ast::null;
        ast::idx idx_func_call_assignment_expression_list = ast::null;
        ast::idx idx_next_postfix_operator = ast::null;
    };

    struct primary_expression {

    };


    struct case_label {
        ast::idx const_expression = ast::null;
    };

    struct default_label {
    };

    //declare
    struct declare_var {

    };

    struct declarator {
        unsigned short int is_ptr = 0;
        ast::idx idx_direct_declarator = ast::null;
    };


    struct initial_declarator_list {
        ast::idx idx_initial_declarator = ast::null;

    };

    struct initial_declarator {
        ast::idx idx_declarator = ast::null;
        ast::idx idx_next_initial_declarator = ast::null;
        ast::idx idx_initializer = ast::null;
    };

    struct declaration_declarator {
        declarator_type type = declarator_type::type_empty;
        declarator_store store = declarator_store::store_empty;
        declarator_limit limit = declarator_limit::limit_empty;
        declarator_sign sign = declarator_sign::sign_signed;
    };

    struct direct_declarator {
        ast::idx idx_identifier = ast::null;
        ast::idx idx_declarator = ast::null;
        ast::idx idx_arguments_type_list = ast::null;
        ast::idx idx_array_declarator = ast::null;
    };

    struct compound_statement {
        ast::idx idx_block_list = ast::null;
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
      //token type_block_by_token = token::invalid;
        ast::idx idx_statement = ast::null;
        ast::idx idx_declaration = ast::null;
        ast::idx idx_next_block = ast::null;
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

    //expression
    struct expr_statement {

    };

    struct if_statement {
        ast::idx idx_expression = ast::null;
        ast::idx idx_if_body = ast::null;
        ast::idx idx_else_body = ast::null;
    };

    struct else_statement {

    };

    struct while_statement {
        ast::idx idx_assignment_expression = ast::null;
        ast::idx idx_compound_statement = ast::null;
    };
    struct do_while_statement {
        ast::idx idx_assign_statement = ast::null;
        ast::idx idx_compound_statement = ast::null;
    };
    struct for_statement {
        ast::idx idx_declaration = ast::null;
        ast::idx idx_conditional_assign_expression = ast::null;
        ast::idx idx_change_assign_expression = ast::null;
        ast::idx idx_compound_statement = ast::null;
    };

    struct break_statement {};

    struct switch_statement {
        ast::idx idx_assign_expression = ast::null;
        ast::idx idx_compound_statement = ast::null;
    };

    struct goto_statement {
        ast::idx idx_identifier = ast::null;
    };

    struct continue_statement {};

    struct return_statement {
        ast::idx idx_assignment_expression = ast::null;
    };


    //
    //those nodes is second step trans nods
    //

    struct enum_definition {

        ast::idx idx_next = ast::null;
    };
    struct struct_declaration {
        ast::idx idx_struct_name = ast::null;
        ast::idx idx_struct_type = ast::null;
        ast::idx idx_next = ast::null;
    };
    struct struct_defination {
        ast::idx idx_struct_name = ast::null;
        ast::idx idx_struct_body = ast::null;
        ast::idx idx_struct_type = ast::null;
        ast::idx idx_next = ast::null;
    };
    struct basic_type_declaration {

        ast::idx idx_next = ast::null;
    };

    struct function_declaration {
        ast::idx idx_function_return_type;
        ast::idx idx_function_declarator;
        ast::idx idx_function_arguments_type_list;
        ast::idx idx_function_name;
        ast::idx idx_function_body;
        ast::idx idx_next = ast::null;
    };

    struct function_definition {

        ast::idx idx_next = ast::null;
    };

    struct arrary_definintion {

        ast::idx idx_next = ast::null;
    };

    struct basic_type_definiton {

        ast::idx idx_next = ast::null;
    };


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
        ast::case_label case_label;
        ast::default_label default_label;
        ast::if_statement if_statement;
        ast::else_statement else_statement;
        ast::while_statement while_statement;
        ast::do_while_statement do_while_statement;
        ast::for_statement for_statement;
        ast::switch_statement switch_statement;
        ast::goto_statement goto_statement;
        ast::continue_statement continue_statement;
        ast::return_statement return_statement;
        ast::break_statement break_statement;
        ast::array_declarator array_declarator;
        //trans node
        ast::function_declaration function_declaration;
        ast::function_definition function_definition;
        ast::arrary_definintion arrary_definintion;
        ast::struct_declaration struct_declaration;
        ast::struct_defination struct_defination;
        ast::basic_type_declaration basic_type_declaration;
        ast::enum_definition enum_definition;
    };

    struct node {
        ast::value value;
        ast::node_type type;
        ast::node_loction node_loction;
        ast::idx next;
    };
}



#endif