#ifndef JUDGE_CHAR
#define JUDGE_CHAR
#include "./AST/judge_char.h"
#endif

#ifndef TOKEN
#define TOKEN
#include "./AST/token.h"
#endif

#ifndef PARSER
#define PARSER
#include "./parser.h"
#endif

#ifndef HEADER
#define HEADER
#include "header.h"
#endif

inline void parser::get_next_char() {
    ++column;
    c = static_cast<char>(file.get());
    return;
}
//inline char parser::peek_next_char() {
//    return static_cast<char>(file.peek());
//}
inline std::tuple<token, std::string> parser::to_number() {
    std::string re;
    bool is_double = false;
    re += c;
    get_next_char();
    while (is_number(c) || c == '.') {
        if (c == '.') is_double = true;
        re += c;
        get_next_char();
    }
    if (is_double) return std::make_tuple(token::class_double, re);
    else           return std::make_tuple(token::class_int, re);
}

inline std::tuple<token, std::string> parser::to_keyWord_or_indentif() {
    std::string re;
    bool is_key_word = false;
    re += c;
    get_next_char();
    while (is_number(c) || is_char(c) || c == '_') {
        re += c;
        get_next_char();
    }
    if (key_words.find(re) == key_words.end()) return std::make_tuple(token::indentif, re);
    else                                       return std::make_tuple(key_words.at(re), re);
}

inline std::tuple<token, std::string> parser::to_string() {
    std::string re;
    get_next_char();
    while (c != '"') {
        re += c;
        get_next_char();
    }
    get_next_char();    //pass string end sign "
    return std::make_tuple(token::class_string, re);
}

inline std::tuple<token, std::string> parser::to_comment() {
    std::string re;
    get_next_char();
    while (c != '\n') {
        re += c;
        get_next_char();
    }
    return std::make_tuple(token::comment, re);
}

inline std::tuple<token, std::string> parser::to_char() {
    get_next_char();
    std::string re;
    re += c;
    get_next_char();
    return std::make_tuple(token::class_char, re);
}
inline std::tuple<token, std::string> parser::next_token() {
    std::string re;
    while (is_useless(c)) {
        if (c == '\n') {
            ++line;
            column = 0;
        }
        get_next_char();
    }
    switch (c) {
        case '[':
            get_next_char();
            return std::make_tuple(token::l_mid_par, "[");

        case ']':
            get_next_char();
            return std::make_tuple(token::r_mid_par, "]");

        case '{':
            get_next_char();
            return std::make_tuple(token::l_big_par, "{");

        case '}':
            get_next_char();
            return std::make_tuple(token::r_big_par, "}");

        case '(':
            get_next_char();
            return std::make_tuple(token::l_par, "(");

        case ')':
            get_next_char();
            return std::make_tuple(token::r_par, ")");

        case ',':
            get_next_char();
            return std::make_tuple(token::comma, ",");

        case '~':
            get_next_char();
            return std::make_tuple(token::bit_not, "~");

        case '+':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::plus_agn, "+=");
            }
            else return std::make_tuple(token::plus, "+");

        case '-':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::minus_agn, "-=");
            }
            else return std::make_tuple(token::minus, "-");

        case '*':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::times_agn, "*=");
            }
            else return std::make_tuple(token::times, "*");

        case '/':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::div_agn, "/=");
            }
            else return std::make_tuple(token::div, "/");

        case '%':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::mod_agn, "%=");
            }
            else return std::make_tuple(token::div, "%");

        case '=':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::equ, "==");
            }
            else return std::make_tuple(token::assign, "=");

        case '!':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::not_equ, "!=");
            }
            else return std::make_tuple(token::log_not, "!");

        case '>':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::great_equ, ">=");
            }
            else return std::make_tuple(token::great, ">");

        case '<':
            get_next_char();
            if (c == '=') {
                get_next_char();
                return std::make_tuple(token::less_equ, "<=");
            }
            else return std::make_tuple(token::less, "<");

        case '&':
            get_next_char();
            if (c == '&') {
                get_next_char();
                return std::make_tuple(token::log_and, "&&");
            }
            else return std::make_tuple(token::bit_and, "&");

        case '#': return to_comment();

        case '"': return to_string();

        case '\'': return to_char();

        default:
            if (is_number(c)) return to_number();
            if (is_char(c))   return to_keyWord_or_indentif();
            break;
    }
    return std::make_tuple(token::invalid, "");
}

inline token parser::get_token(std::tuple<token, std::string> &tuple_) {
    return std::get<0>(tuple_);
}

inline std::string parser::get_value(std::tuple<token, std::string> &tuple_) {
    return std::get<1>(tuple_);
}
//
void parser::output_error_message(token token) {
    std::cout << "error: " << "line: " << line << "column :" << column
              << "key_word" << test_output_token.at(token);
    //end process
    exit(1);
    //^_^'should I write this return, for beaute
    return;
}

//+---------------------------------------
//|function of create node
//|parser statement to node pointer
//+---------------------------------------
std::vector<ast::statement*> parser::parse_block() {
    return {};
}

ast::if_statement* parser::parser_if_statement() {
    //return pointer rather than return a node struct to avoid copy cost
    //memery leak? ^_^' just don't think about it
    auto* node = new ast::if_statement(line, column);

    //吃掉并检查左括号 ^_^' 左括号没有什么用，语法错误怕死循环，
    now_token = next_token();
    if (get_token(now_token) != token::l_par) output_error_message(get_token(now_token));

    //这里并没有取新的token，以后再改
    node->jugement_statement = parser_expression();

    //右括号
    now_token = next_token();
    if (get_token(now_token) != token::r_par) output_error_message(get_token(now_token));

    //解析if ture的语句块
    node->if_block = parse_block();

    //else
    now_token = next_token();
    if (get_token(now_token) == token::key_else) {
        now_token = next_token();
        node->else_block = parse_block();
    }
    return node;
}
ast::while_statement* parser::parser_while_statement() {
    auto* node = new ast::while_statement(line, column);

    //(
    now_token = next_token();
    if (get_token(now_token) != token::l_par) output_error_message(get_token(now_token));

    //条件
    node->jugement_statement = parser_expression();

    //)
    now_token = next_token();
    if (get_token(now_token) != token::r_par) output_error_message(get_token(now_token));

    //while ture 的语句块
    node->while_block = parse_block();
    return node;
}

//return
ast::return_statement* parser::parser_return_statement() {
    auto* node = new ast::return_statement(line, column);

    //return 语句
    node->return_expression = parser_expression();
    return node;
}

//+------------------------------------------
//|function parser expression or statement
//+------------------------------------------

ast::expression* parser::parser_unary_expression() {
    token token = get_token(now_token);
    if (token ==  token::bit_and || token == token::bit_not || token == token::bit_or) {
        auto node = new ast::binary_expr(line, column);
        node->expr_operator = token;
        now_token = next_token();
        node->l_expr = parser_unary_expression();
        return node;
    }
    else if (is_primary_operator(token)) {
        return parser_primary_expression();
    }
    //其实这里永远都不会返回，只是编译器看着烦
    return nullptr;
}

ast::expression* parser::parser_primary_expression() {
    switch (get_token(now_token)) {
        case token::indentif: {
            auto indentif_ = get_value(now_token);
            now_token = next_token();
            switch (get_token(now_token)) {
            //遇到(, 当作函数处理
                case token::l_par: {
                    now_token = next_token();
                    auto* func = new ast::func_call_expr(line, column);
                    func->func_name = indentif_;
                    while (get_token(now_token) != token::r_par) {
                        func->args.push_back(parser_expression());
                        if (get_token(now_token) == token::comma) {
                            now_token = next_token();
                        }
                    }
                    return func;
                    break;
                }
                //遇到[, array[x]
                case token::l_mid_par: {
                    now_token = next_token();
                    auto index = new ast::index_expr(line, column);
                    index->index_name = indentif_;
                    index->index = parser_expression();
                    if (index->index == nullptr) output_error_message(get_token(now_token));
                    now_token = next_token();
                    if (get_token(now_token) != token::r_mid_par) output_error_message(get_token(now_token));
                    now_token = next_token();
                    return index;
                }
                default: {
                    return new ast::indentfi_expr(line, column);
                }
            }
        }
        //int
        case token::class_int: {
            auto* int_ret = new ast::int_expr(line, column);
            int_ret->value = atoi(get_value(now_token).c_str());
            now_token = next_token();
            return int_ret;
        }
        //double
        case token::class_double: {
            auto* double_ret = new ast::double_expr(line, column);
            double_ret->value = atof(get_value(now_token).c_str());
            now_token = next_token();
            return double_ret;
        }
        case token::class_char: {
            auto* char_ret = new ast::char_expr(line, column);
            char_ret->value = get_value(now_token)[0];
            now_token = next_token();
            return char_ret;
        }
        case token::class_bool: {
            auto* bool_ret = new ast::bool_expr(line, column);
            bool_ret->value = (get_token(now_token) == token::key_true);
            now_token = next_token();
            return bool_ret;
        }
        case token::class_string: {
            auto* string_ret = new ast::string_expr(line, column);
            string_ret->value = get_value(now_token);
            now_token = next_token();
            return string_ret;
        }
        case token::key_null: {
            now_token = next_token();
            return new ast::null_expr(line, column);
        }
        case token::l_par: {
            //cross (
            now_token = next_token();
            //parser (expression)
            auto value = parser_expression();
            now_token = next_token();
            return value;
        }
        case token::l_mid_par: {
            //cross [
            now_token = next_token();
            auto* array = new ast::array_expr(line, column);
            if (get_token(now_token) != token::r_mid_par) {
                while (get_token(now_token) != token::r_mid_par) {
                    array->value.push_back(parser_expression());
                    if (get_token(now_token) == token::comma) {
                        now_token = next_token();
                    }
                }
            }
            else {
                now_token = next_token();
                return array;
            }
        }
    }
    //编译器烦人
    return nullptr;
}

ast::expression* parser::parser_expression() {
    auto* node = parser_unary_expression();
    while (is_binary_operator(get_token(now_token))) {
        auto node1 = new ast::binary_expr(line, column);
        node1->l_expr = node;
        node1->expr_operator = get_token(now_token);
        now_token = next_token();
        node1->r_expr = parser_expression();
        node = node1;
    }
    return node;
}















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