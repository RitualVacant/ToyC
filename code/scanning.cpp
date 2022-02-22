#ifndef JUDGE_CHAR
#define JUDGE_CHAR
#include "./header/AST/judge_char.h"
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "./header/AST/token.h"
#endif

#ifndef SCANNING_H
#define SCANNING_H
#include "./header/class/scanning.h"
#endif

inline void scanning::get_next_char() {
    ++column;
    c = static_cast<char>(file.get());
    return;
}
//inline char scanning::peek_next_char() {
//    return static_cast<char>(file.peek());
//}
inline std::tuple<token, std::string> scanning::to_number() {
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

inline std::tuple<token, std::string> scanning::to_keyWord_or_indentif() {
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

inline std::tuple<token, std::string> scanning::to_string() {
    std::string re;
    get_next_char();
    while (c != '"') {
        re += c;
        get_next_char();
    }
    get_next_char();    //pass string end sign "
    return std::make_tuple(token::class_string, re);
}

inline std::tuple<token, std::string> scanning::to_comment() {
    std::string re;
    get_next_char();
    while (c != '\n') {
        re += c;
        get_next_char();
    }
    return std::make_tuple(token::comment, re);
}

inline std::tuple<token, std::string> scanning::to_char() {
    get_next_char();
    std::string re;
    re += c;
    get_next_char();
    return std::make_tuple(token::class_char, re);
}
inline std::tuple<token, std::string> scanning::next_token() {
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

inline token scanning::get_token(std::tuple<token, std::string> &tuple_) {
    return std::get<0>(tuple_);
}

inline std::string scanning::get_value(std::tuple<token, std::string> &tuple_) {
    return std::get<1>(tuple_);
}
//