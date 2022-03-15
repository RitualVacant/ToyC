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
    //now_token = scan->next_token();
    //if (scan->get_token(now_token) != token::l_par)
        //worng::lack_token(line, column, scan->get_token(now_token));

    //这里并没有取新的token，以后再改
    //解析中间的表达式
    //node->jugement_statement = parser_expression();

    //右括号
    //now_token = scan->next_token();
    //if (scan->get_token(now_token) != token::r_par)
        //worng::lack_token(line, column, scan->get_token(now_token));

    //解析if的语句块
    //node->if_block = parse_block();

    //else
    //now_token = scan->next_token();
    //if (scan->get_token(now_token) == token::key_else) {
    //    now_token = scan->next_token();
        //node->else_block = parse_block();
    //}
    return;
}


void parser::parser_while_statement() {
    auto* node = new ast::while_statement(line, column);
    return;
}


//return
void parser::parser_return_statement() {
    //return
    scan->next_token();
    //TODO这里还要解析返回表达式
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
        return;
    }
    else if (is_primary_operator(token_)) {
        parser_primary_expression();
    }
    return;
}

void parser::parser_primary_expression() {

}

void parser::parser_expression_unit() {

}

//转换成后缀表达式
std::vector<std::tuple<token, std::string>>
parser::parser_pre_to_pos() {
    std::vector<std::tuple<token, std::string>> symbol_stack;
    std::vector<std::tuple<token, std::string>> res;//后缀表达式

    while (scan->get_token() != token::end) {
        //TODO 这个next_token的位置
        scan->next_token();
        switch (scan->get_token()) {
            case token::class_int:     //int
            case token::class_double:  //double
            case token::class_char:    //char
            case token::class_bool:    //bool
                symbol_stack.push_back(scan->now_token);

            case token::bit_and: //&
            case token::bit_or:  //|
            case token::bit_not: //~

            case token::log_and: //&&
            case token::log_or:  //||
            case token::log_not: //~

            case token::plus:   //+
                if (preceden(token::plus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                }
                break;

            case token::minus:  //-
                if (preceden(token::minus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                }
                break;

            case token::times:  //*
                if (preceden(token::times) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                }
                break;

            case token::div:    ///
                if (preceden(token::div) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                }
                break;

            case token::mod:    //%
                if (preceden(token::minus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan->now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
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
                break;
        }
    }
    //不为空
    while (!symbol_stack.empty()) {
        res.push_back(symbol_stack.back());
        symbol_stack.pop_back();
    }
    return res;
}

void parser::parser_expression(std::string l_value) {
    std::vector<std::tuple<token, std::string>> pos_experssion = parser_pre_to_pos();

    std::vector<std::string> num_stack;
    std::vector<token> sym_stack;

    std::string arg1;
    std::string arg2;
    for (std::size_t i = 0; i < pos_experssion.size(); ++i) {
        switch(std::get<0>(pos_experssion[i])) {
            case token::indentif:
                num_stack.push_back(std::get<1>(pos_experssion[i]));
                break;
            case token::plus: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = fmt::format("t{}", get_var_time());
                code.push_back(statement(token::plus, arg1, arg2, ans));
                num_stack.push_back(ans);
            }

            case token::minus: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = fmt::format("t{}", get_var_time());
                code.push_back(statement(token::minus, arg1, arg2, ans));
                num_stack.push_back(ans);
            }

            case token::div: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = fmt::format("t{}", get_var_time());
                code.push_back(statement(token::div, arg1, arg2, ans));
                num_stack.push_back(ans);
            }

            case token::mod: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = fmt::format("t{}", get_var_time());
                code.push_back(statement(token::mod, arg1, arg2, ans));
                num_stack.push_back(ans);
            }

            case token::times: {
                arg1 = num_stack.back();
                num_stack.pop_back();
                arg2 = num_stack.back();
                num_stack.pop_back();
                //运算
                std::string ans = fmt::format("t{}", get_var_time());
                code.push_back(statement(token::times, arg1, arg2, ans));
                num_stack.push_back(ans);
            }

            default: {
                fmt::print("\na unknow error at parser::parser_expression()\n");
                exit(0);
            }
        }
    }
//TODO 这个函数不返回值，+= -= 还有这种情况
}
void parser::parser_expression_(std::string name_indentif) {
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
                                sign->asm_mov_auto_r__(size, "ax", sign->find_local(a));
                                rax_init = true;
                            }
                        }
                        sign->asm_any_auto_r__(symbol, size, "ax", b);
                        num_stack.push_back(b);
                    }
                }
                //可能没有运算符只有一个数字
                //TODO
                if (num_stack.size() == 1) {
                    if (std::get<0>(scan->last_token) == token::indentif) {
                        sign->asm_mov_auto_r__(size_class.at(std::get<0>(scan->last_token)), "ax", sign->find_local(num_stack[0]));
                        return;
                    }//是右值
                    else {
                        sign->asm_mov_auto_r__(size_class.at(std::get<0>(scan->last_token)), "ax", num_stack[0]);
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
                            sign->asm_mov_auto___r(size_class.at(sign->find_var_class(a)), sign->find_local(a), "ax");
                            rax_init = true;
                        }
                        //
                        sign->asm_add_auto_r__(size_class.at(sign->find_var_class(b)), "ax", sign->find_local(b));
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
                            sign->asm_mov_auto___r(size_class.at(sign->find_var_class(a)), sign->find_local(a), "ax");
                            rax_init = true;
                        }
                        //
                        sign->asm_add_auto_r__(size_class.at(sign->find_var_class(b)), "ax", sign->find_local(b));
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

void parser::parser_lvalue() {
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

void parser::parser_declare() {
    token class_indentfi = scan->get_token();       //左值类型
    scan->next_token();                             //标识符 | 数组[

    //--------------------------------
    //case: 数组声明 [
    if (scan->get_token() == token::l_mid_par) {
        scan->next_token();                           //
        std::string size_array = scan->get_value();   //数组大小
        switch (class_indentfi) {
            case token::key_int: {
                code.push_back(statement(token::dec_int_arrary, size_array, "", ""));
                break;
            }
            case token::key_char: {
                code.push_back(statement(token::dec_char_arrary, size_array, "", ""));
                break;
            }
            case token::key_bool: {
                code.push_back(statement(token::dec_bool_arrary, size_array, "", ""));
                break;
            }
            case token::key_double:{
                code.push_back(statement(token::dec_double_arrary, size_array, "", ""));
                break;
            }
            default:
                fmt::print("\nunknow error\n");
                exit(0);
        }
        scan->next_token();        //]
        scan->next_token();        //;
        return;
    }

    //--------------------------------
    //case: 标识符
    std::string name_indentfi = scan->get_value();     //标识符名字

    switch (class_indentfi) {
        case token::key_int: {
            code.push_back(statement(token::dec_int, name_indentfi, "", ""));
            break;
        }
        case token::key_char: {
            code.push_back(statement(token::dec_char, name_indentfi, "", ""));
            break;
        }
        case token::key_bool: {
            code.push_back(statement(token::dec_bool, name_indentfi, "", ""));
            break;
        }
        case token::key_double:{
            code.push_back(statement(token::dec_double, name_indentfi, "", ""));
            break;
        }
    }

    //--------------------------------
    //赋值或直接分号结束
    scan->next_token();
    if (scan->get_token() == token::end)      //分号终结
        return;
    if (scan->get_token() == token::assign)   //赋值
        parser_expression(name_indentfi);

    return;
}

//
//解析函数,包括参数和函数体
//

void parser::parser_function() {
    //函数
    std::vector<std::tuple<token, std::string>> argu; //记录参数
    std::string name;                                 //函数名
    token class_return;                               //返回类型

    //--------------------------------
    //函数名
    //前面已经解析了func
    scan->next_token();
    sign->push_func();
    name = scan->get_value();
    sign->asm_func_head(name);//函数头

    //--------------------------------
    scan->next_token();                    // (
    if (scan->get_token() != token::l_par) //没有 (
        worng::lack_token(line, column, {token::l_par, "("});

    //--------------------------------
    //函数参数
    scan->next_token();                            //这里是为了没有参数时直接跳出(.......)中的分析
    while (scan->get_token() != token::r_par) {
        token class_argu = scan->get_token();      //函数参数类型
        scan->next_token();                        //
        std::string name_argu = scan->get_value(); //参数名字
        argu.push_back(std::tuple<token, std::string>(class_argu, name_argu));//参数暂时保存
        scan->next_token();                        //下个参数
        if (scan->get_token() == token::comma)// ,
            scan->next_token();
    }
    //--------------------------------
    //返回值
    scan->next_token();
    if (scan->get_token() != token::ver)
        worng::lack_token(line, column, {token::ver, "->"});
    scan->next_token();
    class_return = scan->get_token();

    //--------------------------------
    //向函数表中插入函数,TODO这一步会对参数分配寄存器
    sign->insert_func(func(name, argu, class_return));

    //--------------------------------
    // {
    scan->next_token();
    if (scan->get_token() != token::l_big_par)
        worng::lack_token(line, column, scan->now_token);

    //--------------------------------
    //函数体
    while (scan->get_token() != token::key_return) {
        parser_statement();
    }

    //--------------------------------
    //函数返回
    parser_return_statement();
    sign->pop_func();

    //--------------------------------
    // }
    scan->next_token();
    if (scan->get_token() != token::r_big_par)
        worng::lack_token(line, column, scan->now_token);
    sign->asm_func_end(); // { ret

    return;
}

//
//解析语句
//
void parser::parser_statement() {
    scan->next_token();
    switch (scan->get_token()) {
            case token::invalid:
        //函数调用或者表达式
            case token::indentif:

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
                break;
            }

        //数值
            case token::class_int: {
                break;
            }
            case token::class_double: {
                break;
            }
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

std::size_t parser::get_var_time() {
    return ++var_time;
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
