#ifndef PARSER_CPP
#define PARSER_CPP

#pragma once
#include "judge_char.h"
#include "token.h"
#include "parser.h"
#include "declataror.h"
#include "synctax_tree.cpp"
#include "inner.cpp"


/*
void
parser::parser_if_statement() {
    scan.next_token(); //if
    scan.next_token(); //(

    std::string value1 = parser_expression();
    token symbol = scan.get_current_token();
    scan.next_token();   //symbol
    std::string value2 = parser_expression();
    std::string lable_if = get_lable();
    //
    switch (symbol) {
        case token::log_and:    //&&
            code.push_back(statement(token::log_and, value1, value2, lable_if));
            break;
        case token::log_or:     //||
            code.push_back(statement(token::log_or, value1, value2, lable_if));
            break;
        case token::equ:        //==
            code.push_back(statement(token::equ, value1, value2, lable_if));
            break;
        case token::not_equ:    //!=
            code.push_back(statement(token::not_equ, value1, value2, lable_if));
            break;
        case token::great_equ:  //>=
            code.push_back(statement(token::great_equ, value1, value2, lable_if));
            break;
        case token::less_equ:   //<=
            code.push_back(statement(token::less_equ, value1, value2, lable_if));
            break;
        case token::great:      //>
            code.push_back(statement(token::great, value1, value2, lable_if));
            break;
        case token::less:       //<
            code.push_back(statement(token::less, value1, value2, lable_if));
            break;
    default:
        break;
    }

    scan.next_token();  // )
    // 不用 } statement里面会跳过

    while (scan.get_current_token() != token::r_big_par) {
        parser_statement();
    }

    code.push_back(statement(token::lable, lable_if, "", ""));

    //else
    if (scan.get_pre_token() == token::key_else) {
        while (scan.get_current_token() != token::r_big_par) {
            parser_statement();
        }
    }
    return;
}
*/

/*
void
parser::parser_while_statement() {
    scan.next_token(); //while
    scan.next_token(); // (
    std::string lable_while_juge = get_lable();
    std::string lable_while_end = get_lable();
    std::string lable_while_block = get_lable();
    code.push_back(statement(token::lable, lable_while_juge, "", ""));

    std::string value1 = parser_expression();
    token symbol = scan.get_current_token();
    scan.next_token();
    std::string value2 = parser_expression();
    //
    switch (symbol) {
        case token::log_and:    //&&
            code.push_back(statement(token::equ, value1, "0", lable_while_end));
            code.push_back(statement(token::equ, value2, "0", lable_while_end));
            break;
        case token::log_or:     //||
            code.push_back(statement(token::not_equ, value1, "0", lable_while_block));
            code.push_back(statement(token::not_equ, value2, "0", lable_while_block));
            break;
        case token::equ:        //==
            code.push_back(statement(token::equ, value1, value2, lable_while_block));
            break;
        case token::not_equ:    //!=
            code.push_back(statement(token::not_equ, value1, value2, lable_while_block));
            break;
        case token::great_equ:  //>=
            code.push_back(statement(token::great_equ, value1, value2, lable_while_block));
            break;
        case token::less_equ:   //<=
            code.push_back(statement(token::less_equ, value1, value2, lable_while_block));
            break;
        case token::great:      //>
            code.push_back(statement(token::great, value1, value2, lable_while_block));
            break;
        case token::less:       //<
            code.push_back(statement(token::less, value1, value2, lable_while_block));
            break;
        default:
            fmt::print("\na unknow error at parser::parser_while_statement()\n");
            exit(0);
            break;
    }

    scan.next_token();
    code.push_back(statement(token::lable, lable_while_block, "", ""));

    while (scan.get_current_token() != token::r_big_par) {
        parser_statement();
    }

    code.push_back(statement(token::jmp, lable_while_juge, "", ""));
    code.push_back(statement(token::lable, lable_while_end, "", ""));
    return;
}
*/

//return
/*
std::string
parser::parser_return_statement() {
    scan.next_token();
    if (scan.get_current_token() == token::end) {
        code.push_back(statement(token::func_return, "", "", ""));
        return "";
    }

    std::string return_value = parser_expression();
    code.push_back(statement(token::func_return, return_value, "", ""));
    return return_value;
}
*/

//+------------------------------------------
//|function parser expression or statement
//+------------------------------------------
//TODO暂时不支持一元表达式

/*
std::unordered_set<token> end_expresion = {
    token::end, token::r_mid_par, token::r_par, token::log_and, token::log_not, token::log_or, token::equ,
    token::not_equ, token::great_equ, token::less_equ, token::great, token::less
};

//转换成后缀表达式
std::vector<std::tuple<token, std::string>>
parser::parser_pre_to_pos() {
    std::vector<std::tuple<token, std::string>> symbol_stack;
    std::vector<std::tuple<token, std::string>> res;//后缀表达式

    while (end_expresion.find(scan.get_current_token()) == end_expresion.end()) {
        //TODO 这个next_token的位置
        switch (scan.get_current_token()) {
            case token::identif: {//identif or arrary unit
                std::string name_indentif = scan.get_current_value();
                token pre_token = scan.get_pre_token();  //预取了
                if (pre_token == token::l_mid_par) {
                    scan.next_token();                   //这里取下一个是因为[ 会在下一次不动
                    std::string array_unit = parser_unit(name_indentif);
                    res.push_back({token::arrary_unit, array_unit});
                }
                else if (pre_token == token::l_par) {
                    scan.next_token();                   //这里也是
                    std::string return_value = parser_func_call(name_indentif);
                    res.push_back({token::identif, return_value});
                }
                else {
                    res.push_back({token::identif, name_indentif});
                }
                break;
            }
            case token::r_int:     //int
            case token::r_double:  //double
            case token::r_char:    //char
            case token::r_bool:    //bool
                res.push_back({scan.get_current_token(), fmt::format("${}", scan.get_current_value())});
                break;

            case token::bit_and: //&
            case token::bit_or:  //|
            case token::bit_not: //~

            case token::log_and: //&&
            case token::log_or:  //||
            case token::log_not: //~

            case token::plus:   //+
                if (symbol_stack.empty() || preceden(token::plus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan.now_token);
                else {
                    while (preceden(token::plus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan.now_token);
                }
                break;

            case token::minus:  //-
                if (symbol_stack.empty() || preceden(token::minus) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan.now_token);
                else {
                    while (preceden(token::minus) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan.now_token);
                }
                break;

            case token::times:
                if (symbol_stack.empty() || preceden(token::times) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan.now_token);
                else {
                    while (preceden(token::times) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan.now_token);
                }
                break;

            case token::div:    ///
                if (symbol_stack.empty() || preceden(token::div) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan.now_token);
                else {
                    while (preceden(token::div) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan.now_token);
                }
                break;

            case token::mod:    //%
                if (symbol_stack.empty() || preceden(token::mod) > preceden(std::get<0>(symbol_stack.back())))
                    symbol_stack.push_back(scan.now_token);
                else {
                    while (preceden(token::mod) <= preceden(std::get<0>(symbol_stack.back()))) {
                        res.push_back(symbol_stack.back());
                        symbol_stack.pop_back();
                    }
                    symbol_stack.push_back(scan.now_token);
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
            case token::times_agn: 
            case token::div_agn:    ///=
            case token::mod_agn:    //%=
            default:
                fmt::print("\nan unknow error at parser::parser_pre_to_pos()\n");
                fmt::print("the token is {} at line:{} colume:{}\n", token_to_string.at(scan.get_current_token()), line, column);
                exit(0);
                break;
        }
        scan.next_token();
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
            case token::identif:
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
*/

/*
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
    //int size;

    scan.next_token();

    while (true) {
        switch (scan.get_current_token()) {
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
                            if (table->find_local(b) == "") {

                            }
                            //是左值
                            else {
                                //table->asm_code_mov_auto_r__(size, "ax", table->find_local(a));
                                rax_init = true;
                            }
                        }
                        //table->asm_code_any_auto_r__(symbol, size, "ax", b);
                        num_stack.push_back(b);
                    }
                }
                //可能没有运算符只有一个数字
                //TODO
                if (num_stack.size() == 1) {
                    if (std::get<0>(scan.last_token) == token::identif) {
                        //table->asm_code_mov_auto_r__(size_class.at(std::get<0>(scan.last_token)), "ax", table->find_local(num_stack[0]));
                        return;
                    }//是右值
                    else {
                        //table->asm_code_mov_auto_r__(size_class.at(std::get<0>(scan.last_token)), "ax", num_stack[0]);
                    }
                    num_stack.pop_back();
                }
            }

        //--------------------------------
        //是内存地址
            //TODO没有全局变量的支持
            case token::identif: {
                //获得标识符的类型
                std::string indentif_ = scan.get_current_value();               //第一个标识符
                token indentif_token = table->find_var_class(indentif_);  //标识符类型
                //size = size_class.at(indentif_token);                    //类型大小

                scan.next_token();
                switch (scan.get_current_token()) {

                //遇到(, 当作函数处理
                    case token::l_par: {
                        auto now_token = scan.next_token();
                        //TODO这里要对函数的参数赋值并且调用函数
                        while (scan.get_current_token(now_token) != token::r_par) {
                            //
                            if (scan.get_current_token(now_token) == token::comma)
                                now_token = scan.next_token();
                        }
                        //看是不是分号结束
                        if (scan.get_current_token() == token::end)
                            return;
                        break;
                    }

                //遇到[, array[x]
                    case token::l_mid_par: {
                        //scan.next_token();
                        //TODO里面的具体细节
                        //TODO下标的大小
                        //parser_expression(); //下标
                        //now_token = scan.next_token();//eat ]
                        continue;
                    }
                //是其他符号
                    default: {
                        //标识符入栈
                        num_stack.push_back(std::get<1>(scan.last_token));
                        continue;
                    }
                }
            }
        //数值
            case token::class_int: {
                num_stack.push_back(scan.get_current_value());
                scan.next_token();
                continue;
            }
            case token::class_double: {
                num_stack.push_back(scan.get_current_value());
                scan.next_token();
                continue;
            }
            case token::class_char: {
                num_stack.push_back(scan.get_current_value());
                scan.next_token();
                continue;
            }
            case token::class_bool: {
                //TODO
                num_stack.push_back(scan.get_current_value());
                scan.next_token();
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
                while (scan.get_current_token() != token::l_par) {

                }
                continue;
            }
        // 运算符
        //TODO 这里应该没有啥用了
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
                            //table->asm_code_mov_auto___r(size_class.at(table->find_var_class(a)), table->find_local(a), "ax");
                            rax_init = true;
                        }
                        num_stack.push_back(b);
                    }
                }
                //最后的符号都要入栈
                symbol_stack.push_back(token::plus);
                scan.next_token();
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
                            //table->asm_code_mov_auto___r(size_class.at(table->find_var_class(a)), table->find_local(a), "ax");
                            rax_init = true;
                        }
                        //
                        //table->asm_code_add_auto_r__(size_class.at(table->find_var_class(b)), "ax", table->find_local(b));
                        //要把弹出的结果入栈，但是这里ax已经起了栈顶的作用，所有就假装 b 塞一个进去
                        num_stack.push_back(b);
                    }
                }
                //最后的符号都要入栈
                symbol_stack.push_back(token::minus);
                scan.next_token();
                continue;
            }
            case token::times:  // *
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
            case token::times_agn:  // *=
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
*/

//TODO 这个函数已经弃用

/*
void
parser::parser_lvalue() {
    token token_class = scan.get_current_token();
    scan.next_token(); //标识符
    scan.next_token(); //
    if (scan.get_current_token() == token::assign) {
        //TODO
        //parser_expression();
        //TODO还有赋值未完成
    }
    return;
}
*/

//DROP
/*
void
parser_declare() {
    token class_indentfi = scan.get_current_token();       //左值类型
    scan.next_token();                             //标识符 | 数组[

    //--------------------------------
    //case: 数组声明 [
    if (scan.get_current_token() == token::l_mid_par) {
        scan.next_token();                           //
        std::string size_array = scan.get_current_value();   //数组大小
        scan.next_token();                           //]
        scan.next_token();
        std::string name_array = scan.get_current_value();                       //数组名
        switch (class_indentfi) {
            case token::key_int: {
                scan.next_token();
                code.push_back(statement(token::dec_int_arrary, name_array, size_array, ""));
                table->push_var(frame(token::class_int_array, name_array));
                break;
            }
            case token::key_char: {
                scan.next_token();
                code.push_back(statement(token::dec_char_arrary, name_array, size_array, ""));
                table->push_var(frame(token::class_char_array, name_array));
                break;
            }
            case token::key_bool: {
                scan.next_token();
                code.push_back(statement(token::dec_bool_arrary, name_array, size_array, ""));
                table->push_var(frame(token::class_bool_array, name_array));
                break;
            }
            case token::key_double: {
                scan.next_token();
                code.push_back(statement(token::dec_double_arrary, name_array, size_array, ""));
                table->push_var(frame(token::class_double_array, name_array));
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
    std::string name_indentfi = scan.get_current_value();     //标识符名字

    switch (class_indentfi) {
        case token::key_int: {
            code.push_back(statement(token::dec_int, name_indentfi, "", ""));
            table->push_var(frame(token::class_int, name_indentfi));
            break;
        }
        case token::key_char: {
            code.push_back(statement(token::dec_char, name_indentfi, "", ""));
            table->push_var(frame(token::class_char, name_indentfi));
            break;
        }
        case token::key_bool: {
            code.push_back(statement(token::dec_bool, name_indentfi, "", ""));
            table->push_var(frame(token::class_bool, name_indentfi));
            break;
        }
        case token::key_double:{
            code.push_back(statement(token::dec_double, name_indentfi, "", ""));
            table->push_var(frame(token::class_double, name_indentfi));
            break;
        }
    }

    //--------------------------------
    //赋值或直接分号结束
    scan.next_token();
    if (scan.get_current_token() == token::end)      //分号终结
        return;
    if (scan.get_current_token() == token::assign) { //赋值
        scan.next_token();                   //=
        std::string r_value = parser_expression();
        code.push_back(statement(token::assign, r_value, "", name_indentfi));
    }

    return;
}
*/

/*
void
parser::parser_function_define_or_declare(std::string func_name, token func_return_type) {
    std::vector<arg_info> argu_table;
    std::string name_func = func_name;
    token class_return    = func_return_type;
    //if next token is ) , won't get into loop to avoid parser argument
    scan.next_token();
    while (scan.get_current_token() != token::r_par) {
        token argu_type = scan.get_current_token();
        scan.next_token();
        std::string name_argu = scan.get_current_value();
        argu_table.push_back(arg_info(argu_type, name_argu, ""));
        scan.next_token();
        // ,
        if (scan.get_current_token() == token::comma) {
            scan.next_token();
        }
    }
    //eat { or ;
    scan.next_token();
    //func declare
    if (scan.get_current_token() == token::end) {
        code.push_back(statement(token::dec_func, name_func, "", ""));
    }
    //func define
    else {
        code.push_back(statement(token::def_func, name_func, "", ""));
        return;
    }
    //func body
    while (scan.get_current_token() != token::key_return) {
        parser_statement();
    }
    //func return
    //std::string return_name = parser_return_statement();
    //eat }
    scan.next_token();
    //clear temporary func symbol stack
    table->clear_symbol_stack();
    //insert func into func table
    //table->insert_func(func(name_func, argu_table, class_return, return_name));
    return;
}
*/

/*
ast::idx
parser::parser_statement() {
    scan.next_token();
    switch (scan.get_current_token()) {
            case token::invalid:
            //函数调用|表达式|数组
            case token::identif: {
                std::string name_indentif = scan.get_current_value();
                std::string r_value;
                scan.next_token();
                //提前判断处理数组元素
                if (scan.get_current_token() == token::l_mid_par) {
                    name_indentif = parser_unit(name_indentif);
                    scan.next_token();
                }
                switch (scan.get_current_token()) {
                    case token::l_par: {    //函数 call
                        parser_func_call(name_indentif);
                        return;
                    }
                    case token::assign: {   //=
                        scan.next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::assign, r_value, "", name_indentif));
                        return;
                    }
                    case token::plus_agn: { //+=
                        scan.next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::minus_agn: {//-=
                        scan.next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::times_agn: { *=
                        scan.next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::div_agn: {  ///=
                        scan.next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    case token::mod_agn: {  //%=
                        scan.next_token();
                        r_value = parser_expression();
                        code.push_back(statement(token::plus_agn, r_value, "", name_indentif));
                        return;
                    }
                    default: {
                        fmt::print("\na unknow error at parser::statement()\n");
                        fmt::print("the token is {} line:{} column:{}\n", token_to_string.at(scan.get_current_token()), line, column);
                        exit(0);
                    }
                }
                return;
            }
        //声明
            case token::key_int:
            case token::key_double:
            case token::key_char:
                //parser_declare();
                return;

        //函数声明
        //  case token::key_func: {
        //      parser_function_define_or_declare();
        //      return;
        //  }
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
    scan.next_token(); // (
    while (scan.get_current_token() != token::r_par) {
        switch (scan.get_current_token()) {
            case token::comma:
                break;
            case token::r_int:
                code.push_back(statement(token::arg_r_int, scan.get_current_value(), "", ""));
                break;
            case token::r_double:
                code.push_back(statement(token::arg_r_double, scan.get_current_value(), "", ""));
                break;
            case token::r_bool:
                code.push_back(statement(token::arg_r_bool, scan.get_current_value(), "", ""));
                break;
            case token::r_char:
                code.push_back(statement(token::arg_r_char, scan.get_current_value(), "", ""));
                break;
            case token::identif: {
                token class_arg = table->find_var_class(scan.get_current_value());
                switch (class_arg) {
                    case token::class_int:
                        code.push_back(statement(token::arg_int_class, scan.get_current_value(), "", ""));
                        break;
                    case token::class_double:
                        code.push_back(statement(token::arg_double_class, scan.get_current_value(), "", ""));
                        break;
                    case token::class_char:
                        code.push_back(statement(token::arg_char_class, scan.get_current_value(), "", ""));
                        break;
                    case token::class_bool:
                        code.push_back(statement(token::arg_bool_class, scan.get_current_value(), "", ""));
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
        scan.next_token();
    }

    std::string return_value = table->find_func_return_var_value(name_func);
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


void
parser::parser_declare_or_function_define_or_declare() {

    auto identifier_type = scan.next_token();
    auto identifier_name = scan.next_token();
    //switch
    //  (   function
    //  [   array
    //  *   pointer
    //  =   basic type of global variable initialize
    //  ;   basic type of global variable
    scan.next_token();
    switch (scan.get_current_token()) {
        case token::l_par: {
            //parser_function_define_or_declare(std::get<1>(identifier_name), std::get<0>(identifier_type));
            break;
        }
        //TODO
        case token::l_mid_par: {
            break;
        }
        case token::times: {

            break;
        }
        case token::assign: {

            break;
        }
        case token::end: {

        }
        default: {
            fmt::print("\nparser::parser_declare_or_function_define_or_declare()\n");
            fmt::print("the token is {}", token_to_string.at(scan.get_current_token()));
            exit(0);
        }
    }
    return;
}
*/

/*
//解析数组元素
std::string
parser::parser_unit(std::string name_array) {
    scan.next_token();                         //[
    //std::string index = parser_expression();
    //scan.next_token();  //]
    //return fmt::format("&{}[{}]",name_array, index);
}
*/

/*
void
parser::print_mid_code() {
    fmt::print("to print in file(f) or on the terminal(t):");
    char c;
    std::cin >> c;
    if (c == 'f') {
        table->file << fmt::format("+------------------+------------------+------------------+------------------+\n");
        table->file << fmt::format("|{:18}|{:18}|{:18}|{:18}|\n", "symbol", "arg1", "arg2", "result");
        table->file << fmt::format("+------------------+------------------+------------------+------------------+\n");
        for (std::size_t i = 0; i < code.size(); ++i) {
            table->file << fmt::format("|{:18}|{:18}|{:18}|{:18}|\n", token_to_string.at(code[i].symbol), code[i].arg1, code[i].arg2, code[i].result);
        }
        table->file << fmt::format("+------------------+------------------+------------------+------------------+\n");
    }
    else if (c == 't') {
        fmt::print("+------------------+------------------+------------------+------------------+\n");
        fmt::print("|{:18}|{:18}|{:18}|{:18}|\n", "symbol", "arg1", "arg2", "result");
        fmt::print("+------------------+------------------+------------------+------------------+\n");
        for (std::size_t i = 0; i < code.size(); ++i) {
            fmt::print("|{:18}|{:18}|{:18}|{:18}|\n", token_to_string.at(code[i].symbol), code[i].arg1, code[i].arg2, code[i].result);
        }
        fmt::print("+------------------+------------------+------------------+------------------+\n");
    }
    else {
        fmt::print("no {} comand, please enter f(file) or t(terminal)\n", c);
        fmt::print("can't output file");
    }
    return;
}
*/

class parser;

//NEW

std::vector<ast::node> &
parser::get_synctax_tree() {
    return tree.get_synctax_tree();
}

void
parser::print_synctax_tree() {
    tree.print_tree("/home/lzj/code/program/script/test/tree.json");
    tree.trans_tree();
    tree.print_tree("/home/lzj/code/program/script/test/tree1.json");
}

ast::idx
parser::parser_declaration_or_definition() {
    ast::idx idx_root = tree.creat_node(ast::node_type::declaration_or_definition);

    tree[idx_root].value.declaration_or_definition.idx_declaration_declarator
    = parser_declaration_declarator();

    tree[idx_root].value.declaration_or_definition.idx_initial_declatator_list
    = parser_initial_declarator();

    //declare
    switch(scan.get_current_token()) {
        //declare
        case token::end: {
            scan.next_token();  //eat ;
            break;
        }
        //define
        case token::l_big_par: {
            tree[idx_root].value.declaration_or_definition.idx_compound_statement
            = parser_compound_statement();
            break;
        }
        default: {
            print_token
            switch_error
        }
    }
    return idx_root;
}

ast::idx
parser::parser_declaration_declarator() {
    //because the order of C's declarator is random
    //this loop will eat token until get identifier or other token
    ast::declaration_declarator node_declarator;
    while (scan.get_current_token() != token::identif
        && scan.get_current_token() != token::l_par
        && scan.get_current_token() != token::times
        ) {
        switch(scan.get_current_token()) {
            //TODO:all type
            case token::key_struct:
                node_declarator.type = ast::declarator_type::type_struct;
                break;
            case token::key_auto:
            case token::key_register:
                break;
            case token::key__Bool:
            case token::key__Complex:
            case token::key__Imaginary:
                //TODO
                break;
            case token::key_static:
                node_declarator.store = ast::declarator_store::store_static;
                break;
            case token::key_extern:
                node_declarator.store = ast::declarator_store::store_extern;
                break;
            case token::key_restrict:
            case token::key_short:
            case token::key_volatile:
            case token::key_signed:
                break;

            case token::key_unsigned: {
                node_declarator.sign = ast::declarator_sign::sign_unsigned;
                break;
            }
            case token::key_int: {
                if (node_declarator.type == ast::declarator_type::type_long_int || node_declarator.type == ast::declarator_type::type_long_long_int) {
                    break;
                }
                node_declarator.type = ast::declarator_type::type_int;
                break;
            }
            case token::key_double: {
                node_declarator.type = ast::declarator_type::type_double;
                break;
            }
            case token::key_char: {
                node_declarator.type = ast::declarator_type::type_char;
                break;
            }
            case token::key_long: {
                if (node_declarator.type == ast::declarator_type::type_int) {
                    node_declarator.type = ast::declarator_type::type_long_int;
                    break;
                }
                if (node_declarator.type == ast::declarator_type::type_long_int) {
                    node_declarator.type = ast::declarator_type::type_long_long_int;
                    break;
                }
                node_declarator.type = ast::declarator_type::type_long_int;
                break;
            }
            case token::key_flaot: {
                node_declarator.type = ast::declarator_type::type_float;
                break;
            }
            default: {
                print_token
                switch_error
            }
        }
        scan.next_token();
    }

    ast::idx idx_root = tree.creat_node(ast::node_type::declaration_declarator);
    tree[idx_root].value.declaration_declarator = node_declarator;
    return idx_root;
}

//DROP
//not creat list node, it's useless
ast::idx
parser::parser_initial_declarator_list() {
    ast::idx idx_root = tree.creat_node(ast::node_type::initial_declarator_list);
    tree[idx_root].value.initial_declarator_list.idx_initial_declarator = parser_initial_declarator();
    if (scan.get_current_token() == token::comma) {
        tree[idx_root].value.initial_declarator.idx_next_initial_declarator = parser_initial_declarator_list();
    }
    return idx_root;
}

ast::idx
parser::parser_initial_declarator() {

    ast::idx idx_root = tree.creat_node(ast::node_type::initial_declarator);
    tree[idx_root].value.initial_declarator.idx_declarator
    = parser_declarator();

    if (scan.get_current_token() == token::assign) {
        scan.next_token();      //eat =
        tree[idx_root].value.initial_declarator.idx_initializer
        = parser_initializer();
    }

    //next initial declarator
    if (scan.get_current_token() == token::comma) {
        scan.next_token();   //eat ,
        tree[idx_root].value.initial_declarator.idx_next_initial_declarator
        = parser_initial_declarator();
    }

    return idx_root;
}

ast::idx
parser::parser_initializer() {
    ast::idx idx_root = tree.creat_node(ast::node_type::initializer);
    if (scan.get_current_token() == token::l_big_par) {
        tree[idx_root].value.initializer.idx_initializer_list
        = parser_initializer_list();
    }
    else {
        tree[idx_root].value.initializer.idx_assignment_expression
        = parser_assignment_expression(ast::null);
    }
    return idx_root;
}

ast::idx
parser::parser_initializer_list() {
    return ast::null;
}

ast::idx
parser::parser_declarator() {
    ast::idx idx_root = tree.creat_node(ast::node_type::declarator);
    while (scan.get_current_token() == token::times) {
        ++tree[idx_root].value.declarator.is_ptr;
        scan.next_token();
    }
    tree[idx_root].value.declarator.idx_direct_declarator = parser_direct_declarator();
    return idx_root;
}

ast::idx
parser::parser_direct_declarator() {
    ast::idx idx_root = tree.creat_node(ast::node_type::direct_declarator);

    if (scan.get_current_token() == token::l_par) {
        scan.next_token();  //eat (
        tree[idx_root].value.direct_declarator.idx_declarator
        = parser_declarator();
        scan.next_token();  //eat )
        if (scan.get_current_token() == token::l_par) {
            scan.next_token();   //eat (
            tree[idx_root].value.direct_declarator.idx_arguments_type_list
            = parser_arguments_type_list();
            scan.next_token();   //eat )
        }
        else {
            tree[idx_root].value.direct_declarator.idx_array_declarator
            = parser_array_declarator();
        }
    }
    else if (scan.get_current_token() == token::identif) {
        tree[idx_root].value.direct_declarator.idx_identifier
        = parser_identifier();

        switch (scan.get_current_token()) {
            case token::l_par: {
                scan.next_token();  //eat (

                tree[idx_root].value.direct_declarator.idx_arguments_type_list
                = parser_arguments_type_list();

                scan.next_token();  //eat )
                break;
            }
            case token::l_mid_par: {
                tree[idx_root].value.direct_declarator.idx_array_declarator
                = parser_array_declarator();
                break;
            }
            //case not parser in this function
            case token::assign:
            case token::r_par:
            case token::l_big_par:
            case token::comma:
            case token::end:
                break;

            default:
                print_token
                switch_error
        }
        /*
        tree[idx_root].value.direct_declarator.idx_identifier
        = parser_identifier();
        switch (scan.get_current_token()) {
            case token::l_par: {
                //eat (
                scan.next_token();
                tree[idx_root].value.direct_declarator.idx_arguments_type_list
                = parser_arguments_type_list();
                //eat )
                scan.next_token();
                break;
            }
            case token::l_mid_par: {
                parser_temporary_1();
                break;
            }
            case token::assign:
            case token::r_par:
            case token::comma:
            case token::end:
                break;
            default:
                print_token
                switch_error
        }
        */
    }
    else {
        fmt::print("an unknow erorr at parser::parser_direct_declarator()");
        exit(0);
    }
    return idx_root;
}

ast::idx
parser::parser_array_declarator() {
    ast::idx idx_root = tree.creat_node(ast::node_type::array_declarator);
    scan.next_token();  //eat [
    tree[idx_root].value.array_declarator.idx_constant
    = parser_assignment_expression(ast::null);
    scan.next_token();  //eat ]

    if (scan.get_current_token() == token::l_mid_par) {
        tree[idx_root].value.array_declarator.idx_next_array_declarator
        = parser_array_declarator();
    }
    return idx_root;
}

ast::idx
parser::parser_identifier() {
    ast::idx idx_root = tree.creat_node(ast::node_type::identifier);
    auto str = scan.get_current_value();
    for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(25), str.size()); ++i) {
        tree[idx_root].value.identifier.name[i] = str[i];
    }
    scan.next_token();
    return idx_root;
}

ast::idx
parser::parser_arguments_type_list() {
    ast::idx idx_root = tree.creat_node(ast::node_type::arguments_type_list);
    tree[idx_root].value.arguments_type_list.idx_argument_declaration
    = parser_arguments_declaration();
    /*
    if (scan.get_current_token() == token::comma) {
    }
    while (scan.get_current_token() != token::r_par) {
        parser_arguments_list();
        //eat ,
        scan.next_token();
    }
    */
    return idx_root;
}
ast::idx
parser::parser_arguments_list() {

    return ast::null;
}
ast::idx
parser::parser_arguments_declaration() {
    ast::idx idx_root = tree.creat_node(ast::node_type::arguments_declaration);

    tree[idx_root].value.arguments_declaration.idx_declararion_declarator
    = parser_declaration_declarator();

    tree[idx_root].value.arguments_declaration.idx_declarator
    = parser_declarator();

    if (scan.get_current_token() == token::comma) {
        scan.next_token();
        tree[idx_root].value.arguments_declaration.idx_next_arguments_declatation
        = parser_arguments_declaration();
    }
    return idx_root;
}


//DROP
ast::idx
parser::parser_conditional_expression() {
    ast::idx idx_root = tree.creat_node(ast::node_type::conditional_expression);
    tree[idx_root].value.conditional_expression.idx_binary_expression
    = parser_binary_expression();
    scan.next_token();      //eat ?
    tree[idx_root].value.conditional_expression.idx_expression
    = parser_expression();
    tree[idx_root].value.conditional_expression.idx_conditional_expression
    = parser_conditional_expression();
    //TODO
    return idx_root;
}

int
parser::operator_priority(token t) {
    switch (t) {
        //end of an experssion
        //return -1
        case token::end:
        case token::assign:
        case token::plus_agn:
        case token::minus_agn:
        case token::times_agn:
        case token::div_agn:
        case token::mod_agn:
        case token::r_shift_agn:
        case token::l_shift_agn:
        case token::bit_and_agn:
        case token::bit_or_agn:
        case token::bit_xor_agn:
        case token::r_par:
        case token::r_mid_par:
        case token::r_big_par:
            return -1;
        case token::log_or:
            return 13;
        case token::log_and:
            return 12;
        case token::bit_or:
            return 11;
        case token::bit_xor:
            return 10;
        case token::bit_and:
            return 9;
        case token::equ:
        case token::not_equ:
            return 8;
        case token::less:
        case token::less_equ:
        case token::great:
        case token::great_equ:
            return 7;
        case token::l_shift:
        case token::r_shift:
            return 6;
        case token::plus:
        case token::minus:
            return 5;
        case token::div:
        case token::mod:
        case token::times:
            return 4;
        default:
            print_token
            switch_error
    }
}

ast::idx
parser::parser_binary_expression() {
    return parser_priority_binary_expression(start_priority);
}

ast::idx
parser::parser_priority_binary_expression(int priority) {
    if (priority == end_priority) {
        return parser_unary_expression();
    }

    ast::idx idx_left_node
    = parser_priority_binary_expression(priority - 1);

    ast::idx idx_root = idx_left_node;

    //creat node
    while (operator_priority(scan.get_current_token()) == priority) {
        idx_root = tree.creat_node(ast::node_type::binary_expression);
        tree[idx_root].value.binary_expression.token_operator
        = scan.get_current_token();

        scan.next_token();

        tree[idx_root].value.binary_expression.idx_left_node
        = idx_left_node;

        tree[idx_root].value.binary_expression.idx_right_node
        = parser_priority_binary_expression(priority - 1);
    }
    //not creat node
    return idx_root;
}

ast::idx
parser::parser_unary_expression() {
    ast::idx idx_root = tree.creat_node(ast::node_type::unary_expression);
    switch (scan.get_current_token()) {
        case token::identif:
        case token::r_int:
        case token::r_double: {
        //TODO
        //case token::string
            tree[idx_root].value.unary_expression.idx_postfix_expression
            = parser_postfix_expression();
            break;
        }

        case token::bit_and:
        case token::times:
        case token::bit_not:
        case token::log_not: {
            scan.next_token();
            tree[idx_root].value.unary_expression.idx_declaration_declatator
            = parser_unary_expression();
            break;
        }

        case token::l_par: {
            switch (scan.get_pre_token()) {
                case token::key__Bool:
                case token::key__Complex:
                case token::key__Imaginary:
                case token::key_int:
                case token::key_long:
                case token::key_short:
                case token::key_flaot:
                case token::key_double:
                case token::key_unsigned:
                case token::key_signed: {
                    scan.next_token();  //eat (

                    tree[idx_root].value.unary_expression.idx_declaration_declatator
                    = parser_declaration_declarator();

                    scan.next_token();  //eat )

                    tree[idx_root].value.unary_expression.idx_unary_expression
                    = parser_unary_expression();
                }

                default: {
                    tree[idx_root].value.unary_expression.idx_postfix_expression
                    = parser_postfix_expression();
                    break;
                }
            }
            break;
        }

        case token::key_sizeof: {
            scan.next_token();
            if (scan.get_current_token() == token::l_par) {
                scan.next_token();
                parser_declaration_declarator();
            }
            else {
                parser_unary_expression();
            }
            break;
        }

        case token::assign:
        case token::plus_agn:
        case token::minus_agn:
        case token::times_agn:
        case token::div_agn:
        case token::mod_agn:
        case token::r_shift_agn:
        case token::l_shift_agn:
        case token::bit_and_agn:
        case token::bit_or_agn:
        case token::bit_xor_agn:
            break;
        default:
            print_token
            switch_error
    }
    return idx_root;
}

ast::idx
parser::parser_postfix_expression() {
    ast::idx idx_root = tree.creat_node(ast::node_type::postfix_expression);

    tree[idx_root].value.postfix_expression.idx_primary_expression
    = parser_primary_expression();

    tree[idx_root].value.postfix_expression.idx_postfix_operator
    = parser_postfix_operator();

    return idx_root;
}

ast::idx
parser::parser_postfix_operator() {
    if (scan.get_current_token() == token::end) {
        return ast::null;
    }

    ast::idx idx_root = tree.creat_node(ast::node_type::postfix_operator);

    tree[idx_root].value.postfix_operator.postfix_operator
    = scan.get_current_token();

    switch (scan.get_current_token()) {
        case token::l_mid_par:
            scan.next_token();  //eat [
            tree[idx_root].value.postfix_operator.idx_array_idx_assignment_expression
            = parser_assignment_expression(ast::null);
            scan.next_token();  //eat ]
            break;

        case token::l_par:
            scan.next_token();  //eat (
            tree[idx_root].value.postfix_operator.idx_func_call_assignment_expression_list
            = parser_assignment_expression_list();
            scan.next_token();  //eat )
            break;

        case token::period:
        case token::ver:
            scan.next_token();
            tree[idx_root].value.postfix_operator.idx_identifier
            = parser_identifier();
            break;

        case token::self_minus:
        case token::self_plus:
            scan.next_token();
            break;

        default:
            //TODO
            return idx_root;
            print_token
            switch_error
    }

    tree[idx_root].value.postfix_operator.idx_next_postfix_operator =
    parser_postfix_operator();

    return idx_root;
}

ast::idx
parser::parser_primary_expression() {
    switch (scan.get_current_token()) {
        case token::identif:
            return parser_identifier();
        case token::r_int:
        case token::r_double:
            //TODO
            return parser_constant();
        //TODO
        //case token::
        case token::l_par: {
            scan.next_token();
            ast::idx idx_expression = parser_expression();
            scan.next_token();
            return idx_expression;
        }
        default:
            print_token
            switch_error
    }
}

ast::idx
parser::parser_assignment_expression_list() {
    return ast::null;
}

ast::idx
parser::parser_compound_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::compound_statement);
    //est {
    scan.next_token();
    tree[idx_root].value.compound_statement.idx_block_list
    = parser_block_list();
    //eat }
    scan.next_token();
    return idx_root;
}

ast::idx
parser::parser_block_list() {
    /*
    ast::idx idx_root = tree.creat_node(ast::node_type::block_list);
    tree[idx_root].value.block_list.idx_block = parser_block();
    return idx_root;
    */
    ast::idx idx_root = parser_block();
    if (idx_root != ast::null) {
        tree[idx_root].value.block.idx_next_block = parser_block_list();
    }
    return idx_root;
}

ast::idx
parser::parser_block() {
    ast::idx idx_root = tree.creat_node(ast::node_type::block);
    switch (scan.get_current_token()) {
        case token::key_int:
        case token::key_double:
        case token::key_long:
        case token::key_char:
        case token::key_flaot:
        case token::key__Bool:
        case token::key__Complex:
        case token::key__Imaginary:
        case token::key_unsigned:
        case token::key_signed:
            tree[idx_root].value.block.idx_declaration
            = parser_declaration_or_definition();
            break;

        case token::key_case:
            tree[idx_root].value.block.idx_statement
            = parser_case_label();
            break;

        case token::key_default:
            tree[idx_root].value.block.idx_statement
            = parser_default_label();
            break;


        case token::identif:
            if(scan.get_pre_token() == token::key_quotation) {
                tree[idx_root].value.block.idx_statement
                = parser_mark_statement();
            }
            else {
                tree[idx_root].value.block.idx_statement
                = parser_expression();
            }
            break;

        case token::key_if:
            tree[idx_root].value.block.idx_statement
            = parser_if_statement();
            break;
        case token::key_while:
            tree[idx_root].value.block.idx_statement
            = parser_while_statement();
            break;
        case token::key_switch:
            tree[idx_root].value.block.idx_statement
            = parser_swtich_statement();
            break;
        case token::key_do:
            tree[idx_root].value.block.idx_statement
            = parser_do_while_statement();
            break;
        case token::key_for:
            tree[idx_root].value.block.idx_statement
            = parser_for_statement();
            break;
        case token::key_break:
            tree[idx_root].value.block.idx_statement
            = parser_break_statement();
            break;
        case token::key_continue:
            tree[idx_root].value.block.idx_statement
            = parser_continue_statement();
            break;
        case token::key_return:
            tree[idx_root].value.block.idx_statement
            = parser_return_statement();

        case token::r_big_par:
            return ast::null;

        default:
            print_token
            switch_error
    }

    return idx_root;
}

ast::idx
parser::parser_case_label() {
    scan.next_token();  //eat case
    ast::idx idx_root = tree.creat_node(ast::node_type::case_label);
    tree[idx_root].value.case_label.const_expression =
    parser_constant();
    scan.next_token();  //eat :
    return idx_root;
}

//TODO
ast::idx
parser::parser_const_expression() {
    return ast::null;
}

ast::idx
parser::parser_default_label() {
    ast::idx idx_root = tree.creat_node(ast::node_type::default_label);
    scan.next_token();  //eat default
    scan.next_token();  //eat :
    return idx_root;
}

ast::idx
parser::parser_mark_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::mark_statement);
    switch (scan.get_current_token()) {
        case token::identif:
            tree[idx_root].value.mark_statement.is_identif = true;
            tree[idx_root].value.mark_statement.idx_identifier
            = parser_identifier();
            break;
        case token::key_case:
            tree[idx_root].value.mark_statement.is_case = true;
            //eat case
            scan.next_token();
            tree[idx_root].value.mark_statement.idx_constant_expression
            = parser_constant();
            //
            break;

        case token::key_default:
            tree[idx_root].value.mark_statement.is_default = true;
            scan.next_token();
            break;
        default: {

        }
    }
    return idx_root;
}

ast::idx
parser::parser_expression() {
    ast::idx idx_root = tree.creat_node(ast::node_type::expression);
    tree[idx_root].value.expression.idx_assignment_expression
    = parser_assignment_expression(ast::null);
    scan.next_token();  //eat ;
    return idx_root;
}

ast::idx
parser::parser_assignment_expression(ast::idx last_assign) {
    ast::idx idx_unary_or_binary_expression = parser_binary_expression();

    switch (scan.get_current_token()) {
        //left is a unary experssion
        case token::assign:
        case token::plus_agn:
        case token::minus_agn:
        case token::div_agn:
        case token::mod_agn:
        case token::times_agn:
        case token::r_shift_agn:
        case token::l_shift_agn:
        case token::bit_and_agn:
        case token::bit_or_agn:
        case token::bit_xor_agn: {
            ast::idx idx_root = tree.creat_node(ast::node_type::assignment_expression);

            tree[idx_root].value.assignment_expression.idx_unary_or_binary_expression
            = idx_unary_or_binary_expression;

            tree[idx_root].value.assignment_expression.idx_next_assignment_expression
            = last_assign;

            tree[idx_root].value.assignment_expression.assignment_type
            = scan.get_current_token();

            scan.next_token();

            return parser_assignment_expression(idx_root);
        }

        //token is ?, a conditional expression
        case token::question_mark: {
            scan.next_token();
            ast::idx idx_root = tree.creat_node(ast::node_type::conditional_expression);

            tree[idx_root].value.conditional_expression.idx_binary_expression
            = idx_unary_or_binary_expression;

            tree[idx_root].value.conditional_expression.idx_expression
            = parser_expression();

            scan.next_token();  //eat :

            tree[idx_root].value.conditional_expression.idx_conditional_expression
            = parser_conditional_expression();

            return idx_root;
        }

        //end of expression ; ) ]
        case token::end:
        case token::r_par:
        case token::r_mid_par:
            if (last_assign != ast::null) {
                tree[last_assign].value.assignment_expression.idx_unary_expression
                = idx_unary_or_binary_expression;

                return last_assign;
            }
            return idx_unary_or_binary_expression;

        default:
            print_token
            switch_error
    }


  //tree[idx_root].value.assignment_expression.idx_unary_expression
  //= parser_unary_expression();

  //tree[idx_root].value.assignment_expression.assignment_type
  //= scan.get_current_token();

  //return parser_assignment_expression(idx_root);
}

ast::idx
parser::parser_statement() {

    return ast::null;
}

ast::idx
parser::parser_mark() {

    return ast::null;
}

ast::idx
parser::parser_constant() {
    ast::idx idx_root = tree.creat_node(ast::node_type::constant);
    std::string str = scan.get_current_value();
    scan.next_token();
    for (std::size_t i = 0; i < std::min(str.size(), ast::array_in_struct_size); ++i) {
        tree[idx_root].value.constant.const_value[i] = str[i];
    }
    return idx_root;
}

ast::idx
parser::parser_not_mark_statement() {

    return ast::null;
}
ast::idx
parser::parser_if_statement() {
    scan.next_token();  //eat if
    scan.next_token();  //eat (
    ast::idx idx_root = tree.creat_node(ast::node_type::if_statement);

    tree[idx_root].value.if_statement.idx_expression
    = parser_assignment_expression(ast::null);

    scan.next_token();  //eat )

    if (scan.get_current_token() == token::l_big_par) {
        tree[idx_root].value.if_statement.idx_if_body
        = parser_compound_statement();
    }
    else {
        tree[idx_root].value.if_statement.idx_if_body
        = parser_block();
    }

    if (scan.get_current_token() == token::key_else) {
        scan.next_token();  //eat else
        if (scan.get_current_token() == token::l_big_par) {
            tree[idx_root].value.if_statement.idx_else_body
            = parser_compound_statement();
        }
        else {
            tree[idx_root].value.if_statement.idx_else_body
            = parser_block();
        }
    }

    return idx_root;
}
ast::idx
parser::parser_swtich_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::switch_statement);

    scan.next_token();  //eat switch
    scan.next_token();  //eat (
    tree[idx_root].value.switch_statement.idx_assign_expression
    = parser_assignment_expression(ast::null);
    scan.next_token();  //eat )

    tree[idx_root].value.switch_statement.idx_compound_statement
    = parser_compound_statement();

    return idx_root;
}
ast::idx
parser::parser_while_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::while_statement);
    scan.next_token();  //eat while
    scan.next_token();  //eat (

    tree[idx_root].value.while_statement.idx_assignment_expression
    = parser_assignment_expression(ast::null);

    scan.next_token();  //eat )

    tree[idx_root].value.while_statement.idx_compound_statement
    = parser_compound_statement();

    return idx_root;
}

ast::idx
parser::parser_do_while_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::do_while_statement);
    scan.next_token();  //eat do

    tree[idx_root].value.do_while_statement.idx_compound_statement
    = parser_compound_statement();

    scan.next_token();  //eat while
    scan.next_token();  //eat (
    tree[idx_root].value.do_while_statement.idx_assign_statement
    = parser_assignment_expression(ast::null);
    scan.next_token();  //eat )

    return idx_root;
}

ast::idx
parser::parser_for_statement() {
    scan.next_token();  //eat for
    scan.next_token();  //eat (
    ast::idx idx_root = tree.creat_node(ast::node_type::for_statement);

    tree[idx_root].value.for_statement.idx_declaration
    = parser_declaration_or_definition();

    //don't need scan.next_token() to eat ;
    //cause in parser_declaration_or_definition had eat ;

    tree[idx_root].value.for_statement.idx_conditional_assign_expression
    = parser_assignment_expression(ast::null);

    scan.next_token();  //eat ;

    tree[idx_root].value.for_statement.idx_change_assign_expression
    = parser_assignment_expression(ast::null);

    scan.next_token();  //eat )

    tree[idx_root].value.for_statement.idx_compound_statement
    = parser_compound_statement();

    return idx_root;
}

ast::idx
parser::parser_break_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::break_statement);
    scan.next_token();  //eat break;
    scan.next_token();  //eat ;
    return idx_root;
}

ast::idx
parser::parser_goto_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::goto_statement);
    scan.next_token();  //eat goto
    tree[idx_root].value.goto_statement.idx_identifier
    = parser_identifier();
    scan.next_token();  //eat;
    return idx_root;
}

ast::idx
parser::parser_continue_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::continue_statement);
    scan.next_token();  //eat continue
    scan.next_token();  //eat ;
    return idx_root;
}
ast::idx
parser::parser_return_statement() {
    ast::idx idx_root = tree.creat_node(ast::node_type::return_statement);
    scan.next_token();  //eat return
    tree[idx_root].value.return_statement.idx_assignment_expression
    = parser_assignment_expression(ast::null);
    scan.next_token();  //eat ;
    return idx_root;
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
