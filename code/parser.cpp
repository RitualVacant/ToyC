#ifndef PARSER_CPP
#define PARSER_CPP

#pragma once
#include "./header/AST/judge_char.h"
#include "./header/AST/token.h"
#include "./header/class/parser.h"


void
parser::parser_if_statement() {
    scan->next_token(); //if
    scan->next_token(); //(

    std::string value1 = parser_expression();
    token symbol = scan->get_token();
    scan->next_token();   //symbol
    std::string value2 = parser_expression();
    //
    switch (symbol) {
        case token::log_and:    //&&
            code.push_back(statement(token::log_and, value1, value2, ""));
            break;
        case token::log_or:     //||
            code.push_back(statement(token::log_or, value1, value2, ""));
            break;
        case token::equ:        //==
            code.push_back(statement(token::equ, value1, value2, ""));
            break;
        case token::not_equ:    //!=
            code.push_back(statement(token::not_equ, value1, value2, ""));
            break;
        case token::great_equ:  //>=
            code.push_back(statement(token::great_equ, value1, value2, ""));
            break;
        case token::less_equ:   //<=
            code.push_back(statement(token::less_equ, value1, value2, ""));
            break;
        case token::great:      //>
            code.push_back(statement(token::great, value1, value2, ""));
            break;
        case token::less:       //<
            code.push_back(statement(token::less, value1, value2, ""));
            break;
    default:
        break;
    }

    scan->next_token();  // )
    // 不用 } statement里面会跳过

    while (scan->get_token() != token::r_big_par) {
        parser_statement();
    }

    std::string lable_1 = get_lable();
    code.push_back(statement(token::lable, lable_1, "", ""));

    //else
    if (scan->get_pre_token() == token::key_else) {
        while (scan->get_token() != token::r_big_par) {
            parser_statement();
        }
    }
    return;
}


void
parser::parser_while_statement() {
    scan->next_token(); //while
    scan->next_token(); // (
    std::string while_head = get_lable();
    code.push_back(statement(token::lable, while_head, "", ""));

    std::string value1 = parser_expression();
    token symbol = scan->get_token();
    scan->next_token();
    std::string value2 = parser_expression();
    //
    switch (symbol) {
        case token::log_and:    //&&
            code.push_back(statement(token::log_and, value1, value2, ""));
            break;
        case token::log_or:     //||
            code.push_back(statement(token::log_or, value1, value2, ""));
            break;
        case token::equ:        //==
            code.push_back(statement(token::equ, value1, value2, ""));
            break;
        case token::not_equ:    //!=
            code.push_back(statement(token::not_equ, value1, value2, ""));
            break;
        case token::great_equ:  //>=
            code.push_back(statement(token::great_equ, value1, value2, ""));
            break;
        case token::less_equ:   //<=
            code.push_back(statement(token::less_equ, value1, value2, ""));
            break;
        case token::great:      //>
            code.push_back(statement(token::great, value1, value2, ""));
            break;
        case token::less:       //<
            code.push_back(statement(token::less, value1, value2, ""));
            break;
        default:
            fmt::print("\na unknow error at parser::parser_while_statement()\n");
            exit(0);
            break;
    }

    scan->next_token();

    while (scan->get_token() != token::r_big_par) {
        parser_statement();
    }

    code.push_back(statement(token::jmp, while_head, "", ""));
    return;
}


//return
std::string
parser::parser_return_statement() {
    //没有返回值
  //if (scan->get_pre_token() == token::end) {
  //    code.push_back(statement(token::func_return, "", "", ""));
  //    return "";
  //}
    scan->next_token();
    if (scan->get_token() == token::end) {
        code.push_back(statement(token::func_return, "", "", ""));
        return "";
    }

    std::string return_value = parser_expression();
    code.push_back(statement(token::func_return, return_value, "", ""));
    return return_value;
}

//+------------------------------------------
//|function parser expression or statement
//+------------------------------------------
//TODO暂时不支持一元表达式

void
parser::parser_primary_expression() {
    return;
}

std::unordered_set<token> end_expresion = {
    token::end, token::r_mid_par, token::r_par, token::log_and, token::log_not, token::log_or, token::equ,
    token::not_equ, token::great_equ, token::less_equ, token::great, token::less
};

//转换成后缀表达式
std::vector<std::tuple<token, std::string>>
parser::parser_pre_to_pos() {
    std::vector<std::tuple<token, std::string>> symbol_stack;
    std::vector<std::tuple<token, std::string>> res;//后缀表达式

    while (end_expresion.find(scan->get_token()) == end_expresion.end()) {
        //TODO 这个next_token的位置
        switch (scan->get_token()) {
            case token::indentif: {//indentif or arrary unit
                std::string name_indentif = scan->get_value();
                token pre_token = scan->get_pre_token();  //预取了
                if (pre_token == token::l_mid_par) {
                    scan->next_token();                   //这里取下一个是因为[ 会在下一次不动
                    std::string array_unit = parser_unit(name_indentif);
                    res.push_back({token::arrary_unit, array_unit});
                }
                else if (pre_token == token::l_par) {
                    scan->next_token();                   //这里也是
                    std::string return_value = parser_func_call(name_indentif);
                    res.push_back({token::indentif, return_value});
                }
                else {
                    res.push_back({token::indentif, name_indentif});
                }
                break;
            }
            case token::r_int:     //int
            case token::r_double:  //double
            case token::r_char:    //char
            case token::r_bool:    //bool
                res.push_back({scan->get_token(), fmt::format("${}", scan->get_value())});
                break;

            case token::bit_and: //&
            case token::bit_or:  //|
            case token::bit_not: //~

            case token::log_and: //&&
            case token::log_or:  //||
            case token::log_not: //~

            case token::plus:   //+
                if (symbol_stack.empty() || preceden(token::plus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::plus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan->now_token);
                }
                break;

            case token::minus:  //-
                if (symbol_stack.empty() || preceden(token::minus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan->now_token);
                }
                break;

            case token::times:  //*
                if (symbol_stack.empty() || preceden(token::times) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::times) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan->now_token);
                }
                break;

            case token::div:    ///
                if (symbol_stack.empty() || preceden(token::div) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::div) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan->now_token);
                }
                break;

            case token::mod:    //%
                if (symbol_stack.empty() || preceden(token::mod) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::mod) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan->now_token);
                }
                break;

            case token::equ:        //==
            case token::not_equ:    //!=
            case token::great_equ:  //>=
            case token::less_equ:   //<=
            case token::great:      //>
            case token::less:       //<


            case token::plus_agn:   //+=
            case token::minus_agn:  //-=
            case token::times_agn:  //*=
            case token::div_agn:    ///=
            case token::mod_agn:    //%=
            default:
                fmt::print("\nan unknow error at parser::parser_pre_to_pos()\n");
                fmt::print("the token is {} at line:{} colume:{}\n", trans_output_token_to_string.at(scan->get_token()), line, column);
                exit(0);
                break;
        }
        scan->next_token();
    }
    //不为空
    while (!symbol_stack.empty()) {
        res.push_back(symbol_stack.back());
        symbol_stack.pop_back();
    }
    return res;
}

std::string
parser::parser_expression() {
    //TODO 这里还有可能是函数调用
    std::vector<std::tuple<token, std::string>> pos_experssion = parser_pre_to_pos();

    std::vector<std::string> num_stack;
    std::vector<token> sym_stack;

    std::string arg1;
    std::string arg2;
    for (std::size_t i = 0; i < pos_experssion.size(); ++i) {
        switch(std::get<0>(pos_experssion[i])) {
            case token::r_int:
            case token::r_double:
            case token::r_char:
            case token::indentif:
            case token::arrary_unit:
                num_stack.push_back(std::get<1>(pos_experssion[i]));
                break;

            case token::plus: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = get_var_time();
                code.push_back(statement(token::plus, arg1, arg2, ans));
                num_stack.push_back(ans);
                break;
            }

            case token::minus: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = get_var_time();
                code.push_back(statement(token::minus, arg1, arg2, ans));
                num_stack.push_back(ans);
                break;
            }

            case token::div: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = get_var_time();
                code.push_back(statement(token::div, arg1, arg2, ans));
                num_stack.push_back(ans);
                break;
            }

            case token::mod: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = get_var_time();
                code.push_back(statement(token::mod, arg1, arg2, ans));
                num_stack.push_back(ans);
                break;
            }

            case token::times: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = get_var_time();
                code.push_back(statement(token::times, arg1, arg2, ans));
                num_stack.push_back(ans);
                break;
            }
            //
            case token::equ:        //==
            case token::not_equ:    //!=
            case token::great_equ:  //>=
            case token::less_equ:   //<=
            case token::great:      //>
            case token::less:       //<
            case token::log_and: //&&
            case token::log_or:  //||

            default: {
                fmt::print("\na unknow error at parser::parser_expression()\n");
                exit(0);
                break;

            }
        }
    }
    return num_stack[0];
}
void
parser::parser_expression_(std::string name_indentif) {
    std::vector<token> symbol_stack;
    std::vector<std::string> num_stack;
    //符号队列 和 数字队列 近似于后缀表达式的正确运算顺序
    std::vector<token> symbol_queue;
    std::vector<std::string> num_queue;

    //--------------------------------
    //判断是不是一元表达式
    //TODO暂时不支持一元表达式

    //TODO下面这两个变量都是要删掉的,为了编译通过
    bool rax_init;
    int size;

    scan->next_token();

    while (true) {
        switch (scan->get_token()) {
        //--------------------------------
        // ;
            case token::end: {
                //可能有一元运算符
                if (symbol_stack.size() != 0) {
                    //TODO 这里可能会有多个值残留在栈里面
                    //TODO 一元表达式无支持
                    while (symbol_stack.size() != 0) {
                        std::string a = num_stack.back();
                        num_stack.pop_back();
                        std::string b = num_stack.back();
                        num_stack.pop_back();
                        token symbol = symbol_stack.back();
                        symbol_stack.pop_back();
                        if (!rax_init) {
                            //是右值
                            if (sign->find_local(b) == "") {

                            }
                            //是左值
                            else {
                                //sign->asm_mov_auto_r__(size, "ax", sign->find_local(a));
                                rax_init = true;
                            }
                        }
                        //sign->asm_any_auto_r__(symbol, size, "ax", b);
                        num_stack.push_back(b);
                    }
                }
                //可能没有运算符只有一个数字
                //TODO
                if (num_stack.size() == 1) {
                    if (std::get<0>(scan->last_token) == token::indentif) {
                        //sign->asm_mov_auto_r__(size_class.at(std::get<0>(scan->last_token)), "ax", sign->find_local(num_stack[0]));
                        return;
                    }//是右值
                    else {
                        //sign->asm_mov_auto_r__(size_class.at(std::get<0>(scan->last_token)), "ax", num_stack[0]);
                    }
                    num_stack.pop_back();
                }
            }

        //--------------------------------
        //是内存地址
            //TODO没有全局变量的支持
            case token::indentif: {
                //获得标识符的类型
                std::string indentif_ = scan->get_value();               //第一个标识符
                token indentif_token = sign->find_var_class(indentif_);  //标识符类型
                //size = size_class.at(indentif_token);                    //类型大小

                scan->next_token();
                switch (scan->get_token()) {

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
                        //TODO下标的大小
                        //parser_expression(); //下标
                        //now_token = scan->next_token();//eat ]
                        continue;
                    }
                //是其他符号
                    default: {
                        //标识符入栈
                        num_stack.push_back(std::get<1>(scan->last_token));
                        continue;
                    }
                }
            }
        //数值
            case token::class_int: {
                num_stack.push_back(scan->get_value());
                scan->next_token();
                continue;
            }
            case token::class_double: {
                num_stack.push_back(scan->get_value());
                scan->next_token();
                continue;
            }
            case token::class_char: {
                num_stack.push_back(scan->get_value());
                scan->next_token();
                continue;
            }
            case token::class_bool: {
                //TODO
                num_stack.push_back(scan->get_value());
                scan->next_token();
                continue;
            }
            case token::key_null: {
                //TODO
            }
        // (
            case token::l_par: {
                symbol_stack.push_back(token::l_par);
                continue;
            }
        // )
            case token::r_par: {
                while (scan->get_token() != token::l_par) {

                }
                continue;
            }
        // 运算符
            case token::bit_and: //&
            case token::bit_or:  //|
            case token::bit_not: //~
            case token::log_and: //&&
            case token::log_or:  //||
            case token::log_not: //~
            case token::plus: { //+
                if (!symbol_stack.empty() && preceden(token::plus) <= preceden(symbol_stack.back())) {
                    while (!symbol_stack.empty() && preceden(token::plus) <= preceden(symbol_stack.back())) {
                        std::string a = num_stack.back();
                        num_stack.pop_back();
                        std::string b = num_stack.back();
                        num_stack.pop_back();
                        token symbol = symbol_stack.back();
                        symbol_stack.pop_back();
                        //如果rax没有初始化
                        if (!rax_init) {
                            //sign->asm_mov_auto___r(size_class.at(sign->find_var_class(a)), sign->find_local(a), "ax");
                            rax_init = true;
                        }
                        //
                        //sign->asm_add_auto_r__(size_class.at(sign->find_var_class(b)), "ax", sign->find_local(b));
                        //要把弹出的结果入栈，但是这里ax已经起了栈顶的作用，所有就假装 b 塞一个进去
                        num_stack.push_back(b);
                    }
                }
                //最后的符号都要入栈
                symbol_stack.push_back(token::plus);
                scan->next_token();
                continue;
            }

            case token::minus: {//-
                if (!symbol_stack.empty() && preceden(token::minus) <= preceden(symbol_stack.back())) {
                    while (!symbol_stack.empty() && preceden(token::minus) <= preceden(symbol_stack.back())) {
                        std::string a = num_stack.back();
                        num_stack.pop_back();
                        std::string b = num_stack.back();
                        num_stack.pop_back();
                        token symbol = symbol_stack.back();
                        symbol_stack.pop_back();
                        //如果rax没有初始化
                        if (!rax_init) {
                            //sign->asm_mov_auto___r(size_class.at(sign->find_var_class(a)), sign->find_local(a), "ax");
                            rax_init = true;
                        }
                        //
                        //sign->asm_add_auto_r__(size_class.at(sign->find_var_class(b)), "ax", sign->find_local(b));
                        //要把弹出的结果入栈，但是这里ax已经起了栈顶的作用，所有就假装 b 塞一个进去
                        num_stack.push_back(b);
                    }
                }
                //最后的符号都要入栈
                symbol_stack.push_back(token::minus);
                scan->next_token();
                continue;
            }
            case token::times:  //*
                while (preceden(token::plus) > preceden(symbol_stack.back())) {
                }
                continue;
            case token::div:    ///
                while (preceden(token::plus) > preceden(symbol_stack.back())) {
                }
                continue;
            case token::mod:    //%
                while (preceden(token::plus) > preceden(symbol_stack.back())) {
                }
                continue;
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
            default:
                break;
        }
        //
    }
    //编译器烦人
    return;
}

void
parser::parser_lvalue() {
    token token_class = scan->get_token();
    scan->next_token(); //标识符
    scan->next_token(); //
    if (scan->get_token() == token::assign) {
        //TODO
        //parser_expression();
        //TODO还有赋值未完成
    }
    return;
}

void
parser::parser_declare() {
    token class_indentfi = scan->get_token();       //左值类型
    scan->next_token();                             //标识符 | 数组[

    //--------------------------------
    //case: 数组声明 [
    if (scan->get_token() == token::l_mid_par) {
        scan->next_token();                           //
        std::string size_array = scan->get_value();   //数组大小
        scan->next_token();                           //]
        scan->next_token();
        std::string name_array = scan->get_value();                       //数组名
        switch (class_indentfi) {
            case token::key_int: {
                scan->next_token();
                code.push_back(statement(token::dec_int_arrary, name_array, size_array, ""));
                sign->push_var(frame(token::class_int_array, name_array));
                break;
            }
            case token::key_char: {
                scan->next_token();
                code.push_back(statement(token::dec_char_arrary, name_array, size_array, ""));
                sign->push_var(frame(token::class_char_array, name_array));
                break;
            }
            case token::key_bool: {
                scan->next_token();
                code.push_back(statement(token::dec_bool_arrary, name_array, size_array, ""));
                sign->push_var(frame(token::class_bool_array, name_array));
                break;
            }
            case token::key_double: {
                scan->next_token();
                code.push_back(statement(token::dec_double_arrary, name_array, size_array, ""));
                sign->push_var(frame(token::class_double_array, name_array));
                break;
            }
            default:
                fmt::print("\nunknow error\n");
                exit(0);
        }
        return;
    }

    //--------------------------------
    //case: 标识符
    std::string name_indentfi = scan->get_value();     //标识符名字

    switch (class_indentfi) {
        case token::key_int: {
            code.push_back(statement(token::dec_int, name_indentfi, "", ""));
            sign->push_var(frame(token::class_int, name_indentfi));
            break;
        }
        case token::key_char: {
            code.push_back(statement(token::dec_char, name_indentfi, "", ""));
            sign->push_var(frame(token::class_char, name_indentfi));
            break;
        }
        case token::key_bool: {
            code.push_back(statement(token::dec_bool, name_indentfi, "", ""));
            sign->push_var(frame(token::class_bool, name_indentfi));
            break;
        }
        case token::key_double:{
            code.push_back(statement(token::dec_double, name_indentfi, "", ""));
            sign->push_var(frame(token::class_double, name_indentfi));
            break;
        }
    }

    //--------------------------------
    //赋值或直接分号结束
    scan->next_token();
    if (scan->get_token() == token::end)      //分号终结
        return;
    if (scan->get_token() == token::assign) { //赋值
        scan->next_token();                   //=
        std::string r_value = parser_expression();
        code.push_back(statement(token::assign, r_value, "", name_indentfi));
    }

    return;
}

//
//解析函数,包括参数和函数体
//

void
parser::parser_function() {
    //函数
    std::vector<arg_info> argu_table; //记录参数
    std::string name_func;                            //函数名
    token class_return;                               //返回类型
    //函数名 前面已经解析了func
    scan->next_token();
    name_func = scan->get_value();
    //函数头
    code.push_back(statement(token::dec_func, name_func, "", ""));
    scan->next_token();                    // (
    //函数参数
    scan->next_token();                            //这里是为了没有参数时直接跳出(.......)中的分析
    while (scan->get_token() != token::r_par) {
        token class_argu = scan->get_token();      //函数参数类型
        scan->next_token();                        //
        std::string name_argu = scan->get_value(); //参数名字
        argu_table.push_back(arg_info(class_argu, name_argu, ""));//参数暂时保存
        scan->next_token();                        //下个参数
        if (scan->get_token() == token::comma)     // ,
            scan->next_token();
    }
    //返回值
    scan->next_token(); //->
    scan->next_token(); //返回类型
    class_return = scan->get_token();
    // {
    //scan->next_token();
    //函数体
    while (scan->get_token() != token::key_return) {
        parser_statement();
    }
    //函数返回
    std::string return_name = parser_return_statement();
    // }
    scan->next_token();
    //把函数栈清空
    sign->clear_symbol_stack();

    //向函数表中插入函数
    sign->insert_func(func(name_func, argu_table, class_return, return_name));
    return;
}

//
//解析语句
//
void
parser::parser_statement() {
    scan->next_token();
    switch (scan->get_token()) {
            case token::invalid:
        //函数调用|表达式|数组
            case token::indentif: {
                std::string name_indentif = scan->get_value();
                std::string r_value;
                scan->next_token();
                //提前判断处理数组元素
                if (scan->get_token() == token::l_mid_par) {
                    name_indentif = parser_unit(name_indentif);
                    scan->next_token();
                }
                switch (scan->get_token()) {
                    case token::l_par: {    //函数 call
                        parser_func_call(name_indentif);
                        return;
                    }
                    case token::assign: {   //=
                        scan->next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::assign, r_value, "", name_indentif));
                        return;
                    }
                    case token::plus_agn: { //+=
                        scan->next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::minus_agn: {//-=
                        scan->next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::times_agn: {//*=
                        scan->next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::div_agn: {  ///=
                        scan->next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::mod_agn: {  //%=
                        scan->next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    default: {
                        fmt::print("\na unknow error at parser::statement()\n");
                        fmt::print("the token is {} line:{} column:{}\n", trans_output_token_to_string.at(scan->get_token()), line, column);
                        exit(0);
                    }
                }
                return;
            }
        //声明
            case token::key_int:
            case token::key_double:
            case token::key_char:
            case token::key_bool:
                parser_declare();
                return;

        //函数声明
            case token::key_func: {
                parser_function();
                return;
            }
        //右值
            case token::r_char:
            case token::r_int:
            case token::r_double:
                parser_expression();

            case token::bit_and: //&
            case token::bit_or:  //|
            case token::bit_not: //~
            case token::log_and: //&&
            case token::log_or:  //||
            case token::log_not: //~

            case token::equ:        //==
            case token::not_equ:    //!=
            case token::great_equ:  //>=
            case token::less_equ:   //<=
            case token::great:      //>
            case token::less:       //<
            case token::assign:     //=

            case token::comment:        //#
            case token::key_if: {       //if
                parser_if_statement();
                return;
            }
            case token::key_true:       //true
            case token::key_false:      //false

            case token::key_while: {    //while
                parser_while_statement();
                return;
            }
            case token::key_for:        //for
            case token::key_null:       //null
            case token::key_return:     //return
            case token::key_break:      //break
            case token::key_continue:   //continue

            default:
            break;
    }
}

std::string
parser::parser_func_call(std::string name_func) {
    code.push_back(statement(token::call_func, name_func, "", ""));
    scan->next_token(); // (
    while (scan->get_token() != token::r_par) {
        switch (scan->get_token()) {
            case token::comma:
                break;
            case token::r_int:
                code.push_back(statement(token::arg_r_int, scan->get_value(), "", ""));
                break;
            case token::r_double:
                code.push_back(statement(token::arg_r_double, scan->get_value(), "", ""));
                break;
            case token::r_bool:
                code.push_back(statement(token::arg_r_bool, scan->get_value(), "", ""));
                break;
            case token::r_char:
                code.push_back(statement(token::arg_r_char, scan->get_value(), "", ""));
                break;
            case token::indentif: {
                token class_arg = sign->find_var_class(scan->get_value());
                switch (class_arg) {
                    case token::class_int:
                        code.push_back(statement(token::arg_int_class, scan->get_value(), "", ""));
                        break;
                    case token::class_double:
                        code.push_back(statement(token::arg_double_class, scan->get_value(), "", ""));
                        break;
                    case token::class_char:
                        code.push_back(statement(token::arg_char_class, scan->get_value(), "", ""));
                        break;
                    case token::class_bool:
                        code.push_back(statement(token::arg_bool_class, scan->get_value(), "", ""));
                        break;
                    default:
                        fmt::print("\na unknow error at parser::parser_func_call\n");
                        exit(0);
                }
                break;
            }
            default: {
                fmt::print("\na unknow error at parser::parser_func_call() line:{} colume:{}\n", line, column);
                exit(0);
            }
        }
        scan->next_token();
    }

    std::string return_value = sign->find_func_return_var_value(name_func);
    code.push_back(statement(token::call_func_end, "", "", ""));
    return return_value;
}

std::string
parser::get_var_time() {
    std::string re = fmt::format("#{}", var_time);
    ++var_time;
    return re;
}

std::string
parser::get_lable() {
    std::string re = fmt::format("lable{}", lable);
    ++lable;
    return re;
}

//解析数组元素
std::string
parser::parser_unit(std::string name_array) {
    scan->next_token();                         //[
    std::string index = parser_expression();
    //scan->next_token();  //]
    return fmt::format("&{}[{}]",name_array, index);
}

void
parser::print_mid_code() {
    fmt::print("to print in file(f) or on the teriminal(t):");
    char c;
    std::cin >> c;
    if (c == 'f') {
        sign->file << fmt::format("+------------------+------------------+------------------+------------------+\n");
        sign->file << fmt::format("|{:18}|{:18}|{:18}|{:18}|\n", "symbol", "arg1", "arg2", "result");
        sign->file << fmt::format("+------------------+------------------+------------------+------------------+\n");
        for (std::size_t i = 0; i < code.size(); ++i) {
            sign->file << fmt::format("|{:18}|{:18}|{:18}|{:18}|\n", trans_output_token_to_string.at(code[i].symbol), code[i].arg1, code[i].arg2, code[i].result);
        }
        sign->file << fmt::format("+------------------+------------------+------------------+------------------+\n");
    }
    else {
        fmt::print("+------------------+------------------+------------------+------------------+\n");
        fmt::print("|{:18}|{:18}|{:18}|{:18}|\n", "symbol", "arg1", "arg2", "result");
        fmt::print("+------------------+------------------+------------------+------------------+\n");
        for (std::size_t i = 0; i < code.size(); ++i) {
            fmt::print("|{:18}|{:18}|{:18}|{:18}|\n", trans_output_token_to_string.at(code[i].symbol), code[i].arg1, code[i].arg2, code[i].result);
        }
        fmt::print("+------------------+------------------+------------------+------------------+\n");
    }
    return;
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
