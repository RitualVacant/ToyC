#ifndef PARSER_CPP
#define PARSER_CPP

#pragma once
#include "./header/AST/judge_char.h"
#include "./header/AST/token.h"
#include "./header/class/parser.h"

//+---------------------------------------
//|function of create node
//|parser statement to node pointer
//+---------------------------------------


void parser::parser_if_statement() {

    //return pointer rather than return a node struct to avoid copy cost
    //memery leak? ^_^' just don't think about it
    auto* node = new ast::if_statement(line, column);

    //吃掉并检查左括号 ^_^' 左括号没有什么用，语法错误怕死循环，
    now_token = scan->next_token();
    if (scan->get_token(now_token) != token::l_par)
        //worng::lack_token(line, column, scan->get_token(now_token));

    //这里并没有取新的token，以后再改
    //解析中间的表达式
    //node->jugement_statement = parser_expression();

    //右括号
    now_token = scan->next_token();
    if (scan->get_token(now_token) != token::r_par)
        //worng::lack_token(line, column, scan->get_token(now_token));

    //解析if的语句块
    //node->if_block = parse_block();

    //else
    //now_token = scan->next_token();
    if (scan->get_token(now_token) == token::key_else) {
        now_token = scan->next_token();
        //node->else_block = parse_block();
    }
    return;
}


void parser::parser_while_statement() {
    auto* node = new ast::while_statement(line, column);

    //(
    now_token = scan->next_token();
    //if (scan->get_token(now_token) != token::l_par) output_error_message(scan->get_token(now_token));

    //条件
    //node->jugement_statement = parser_expression();

    //)
    now_token = scan->next_token();
    //if (scan->get_token(now_token) != token::r_par) output_error_message(scan->get_token(now_token));

    //while ture 的语句块
    //node->while_block = parse_block();
    return;
}


//return
void parser::parser_return_statement() {
    //return
    sign->asm_func_end();
    scan->next_token();
    return;
}

//+------------------------------------------
//|function parser expression or statement
//+------------------------------------------
//TODO暂时不支持一元表达式
void parser::parser_unary_expression() {
    token token_ = scan->get_token();
    //一元表达式
    //
    if (token_ ==  token::bit_and || token_ == token::bit_not || token_ == token::bit_or) {
        //
        //
        //
        //
        return;
    }
    else if (is_primary_operator(token_)) {
        parser_primary_expression();
    }
    //其实这里永远都不会返回，只是编译器看着烦
    return;
}

void parser::parser_primary_expression() {
    switch (scan->get_token(now_token)) {
        case token::indentif: {
            auto indentif_ = scan->get_value(now_token);
            now_token = scan->next_token();
            switch (scan->get_token(now_token)) {
            //遇到(, 当作函数处理
                case token::l_par: {
                    auto now_token = scan->next_token();
                    //TODO这里要对函数的参数赋值并且调用函数
                    while (scan->get_token(now_token) != token::r_par) {
                        //
                        if (scan->get_token(now_token) == token::comma)
                            now_token = scan->next_token();
                    }
                    //看是不是分号结束
                    if (scan->get_token() == token::end)
                        return;
                    break;
                }
            //遇到[, array[x]
                case token::l_mid_par: {
                    //scan->next_token();
                    //TODO里面的具体细节
                    parser_expression(); //下标
                    now_token = scan->next_token();//eat ]
                    return;
                }
                default: {
                    return;//new ast::indentfi_expr(line, column);
                }
            }
        }
    //int
        case token::class_int: {
            return;// int_ret;
        }
    //double
        case token::class_double: {
            return;// double_ret;
        }
        case token::class_char: {
            return;//char_ret;
        }
        case token::class_bool: {
            return;// bool_ret;
        }
        case token::class_string: {
            return;// string_ret;
        }
        case token::key_null: {
            return;// new ast::null_expr(line, column);
        }
        //TODO应该是不保留这个左括号的
        case token::l_par: {
            //cross (
            now_token = scan->next_token();
            //parser (expression)
            //auto value = parser_expression();
            now_token = scan->next_token();
            return;// value;
        }
        case token::l_mid_par: {
        }
    }
    //编译器烦人
    return;
}


void parser::parser_expression() {
    //TODO这里没有完工
    parser_unary_expression();
    parser_primary_expression();
    return;
}

void parser::parser_lvalue() {
    token token_class = scan->get_token();
    scan->next_token(); //标识符
    scan->next_token(); //
    if (scan->get_token() == token::assign) {
        parser_expression();
        //TODO还有赋值未完成
    }
    return;
}

void parser::parser_declare() {
    token token_class = scan->get_token(); //左值类型
    scan->next_token();                    //int 后的标识符,或者是数组[
    //数组声明
    if (scan->get_token() == token::l_mid_par) {
        scan->next_token();
        int size_array = stoi(scan->get_value());
        scan->next_token(); //]
        //TODO数组没有声明时的赋值
        return;
    }
    //基本类型
    if (sign->find(scan->now_token))       //重复定义,报错直接退出
        worng::redefine(line, column, scan->now_token);
    sign->push_token(scan->now_token); //入栈
    scan->next_token();
    //赋值或直接分号结束
    if (scan->get_token() == token::end) //分号终结
        return;
    if (scan->get_token() == token::assign) {
        parser_expression();
        //TODO应该想怎么赋值，
    }
    parser_statement();//回到初始状态
    return;
}

void parser::parser_function() {
    //函数名
    scan->next_token();
    sign->push_func(scan->now_token);
    //函数参数
    scan->next_token();                    // (
    if (scan->get_token() != token::l_par) //没有 (
        worng::lack_token(line, column, scan->now_token);
    scan->next_token();
    while (scan->get_token() != token::r_par) {
        scan->next_token();                  //函数参数类型
        token class_argu = scan->get_token();//记录参数类型
        scan->next_token();                  //函数参数声明符
        sign->push_token(std::tuple<token, std::string>(class_argu, scan->get_value()));
        scan->next_token();                   //下个参数
        if (scan->get_token() == token::comma)// ,
            scan->next_token();
    }
    // {
    scan->next_token();
    if (scan->get_token() != token::l_big_par)
        worng::lack_token(line, column, scan->now_token);
    //函数体
    while (scan->get_token() != token::key_return) {
        parser_statement();
    }
    //函数返回
    parser_return_statement();
    sign->pop_func();
    // }
    scan->next_token();
    return;
}

void parser::parser_statement() {
    scan->next_token();
    switch (scan->get_token()) {
            case token::invalid:

        //声明
            case token::key_int:
            case token::key_double:
            case token::key_char:
            case token::key_bool: {
                parser_declare();
                if (scan->get_token() == token::assign) {
                    parser_expression();
                    //TODO这里要补上负值语句
                }
                break;
            }

        //函数声明
            case token::key_func: {
                parser_function();
                break;
            }

        //数值
        //int
            case token::class_int: {
                break;
            }
        //double
            case token::class_double: {
                break;
            }

            case token::class_string:  //string

            case token::class_char:    //char

            case token::bit_and: //&
            case token::bit_or:  //|
            case token::bit_not: //~
            case token::log_and: //&&
            case token::log_or:  //||
            case token::log_not: //~
            case token::plus:   //+
            case token::minus:  //-
            case token::times:  //*
            case token::div:    ///
            case token::mod:    //%
            case token::equ:        //==
            case token::not_equ:       //!=
            case token::great_equ:  //>=
            case token::less_equ:   //<=
            case token::great:      //>
            case token::less:       //<
            case token::assign:     //=
            case token::plus_agn:   //+=
            case token::minus_agn:  //-=
            case token::times_agn:  //*=
            case token::div_agn:    ///=
            case token::mod_agn:    //%=
            case token::l_par:       //(
            case token::r_par:       //)
            case token::l_mid_par:   //[
            case token::r_mid_par:   //]
            case token::l_big_par:   //{
            case token::r_big_par:   //}
            case token::comma:   //,
            case token::comment: //#
            case token::indentif:  //标识符
            case token::key_if:         //if

            case token::key_else:       //else
            case token::key_true:       //true
            case token::key_false:      //false
            case token::key_while:      //while
            case token::key_for:        //for
            case token::key_null:       //null
            case token::key_return:     //return
            case token::key_break:      //break
            case token::key_continue:   //continue

            default:
            break;
    }
}

#endif

//⠄⠄⠄⠄⢠⣿⣿⣿⣿⣿⢻⣿⣿⣿⣿⣿⣿⣿⣿⣯⢻⣿⣿⣿⣿⣆
//⠄⠄⣼⢀⣿⣿⣿⣿⣏⡏⠄⠹⣿⣿⣿⣿⣿⣿⣿⣿⣧⢻⣿⣿⣿⣿
//⠄⠄⡟⣼⣿⣿⣿⣿⣿⠄⠄⠄⠈⠻⣿⣿⣿⣿⣿⣿⣿⣇⢻⣿⣿⣿⣿
//⠄⢰⠃⣿⣿⠿⣿⣿⣿⠄⠄⠄⠄⠄⠄⠙⠿⣿⣿⣿⣿⣿⠄⢿⣿⣿⣿⡄
//⠄⢸⢠⣿⣿⣧⡙⣿⣿⡆⠄⠄⠄⠄⠄⠄⠄⠈⠛⢿⣿⣿⡇⠸⣿⡿⣸⡇
//⠄⠈⡆⣿⣿⣿⣿⣦⡙⠳⠄⠄⠄⠄⠄⠄⢀⣠⣤⣀⣈⠙⠃⠄⠿⢇⣿⡇
//⠄⠄⡇⢿⣿⣿⣿⣿⡇⠄⠄⠄⠄⠄⣠⣶⣿⣿⣿⣿⣿⣿⣷⣆⡀⣼⣿
//⠄⠄⢹⡘⣿⣿⣿⢿⣷⡀⠄⢀⣴⣾⣟⠉⠉⠉⠉⣽⣿⣿⣿⣿⠇⢹⣿
//⠄⠄⠄⢷⡘⢿⣿⣎⢻⣷⠰⣿⣿⣿⣿⣦⣀⣀⣴⣿⣿⣿⠟⢫⡾⢸⡟
//⠄⠄⠄⠄⠻⣦⡙⠿⣧⠙⢷⠙⠻⠿⢿⡿⠿⠿⠛⠋⠉⠄⠂⠘⠁⠞
//⠄⠄⠄⠄⠄⠈⠙⠑⣠⣤⣴⡖⠄⠿⣋⣉⣉⡁⠄⢾⣦
