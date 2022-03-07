#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef TUPLE
#define TUPLE
#include <tuple>
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "./../AST/token.h"
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef AST_H
#define AST_H
#include "./../AST/ast.h"
#endif

#ifndef FMT_CORE_H
#define FMT_CORE_H
#include "fmt/core.h"
#endif

#ifndef FMT_COLOR_H
#define FMT_COLOR_H
#include "fmt/color.h"
#endif


class scanning {
    private:
        unsigned int line = 0;
        unsigned int column = 0;
        char c;
        std::size_t num_token = 0;  //the number of token in this script file
        std::string file_path;
        std::tuple<token, std::string> &&now_token = {};

        void get_next_char();

        std::tuple<token, std::string> to_number();
        std::tuple<token, std::string> to_keyWord_or_indentif();
        std::tuple<token, std::string> to_comment();
        std::tuple<token, std::string> to_char();
        std::tuple<token, std::string> to_string();

    public:
        std::fstream file;

        explicit scanning(std::string &file_path_) : file_path (file_path_) {
            file.open(file_path);
            if (!file.is_open()) {
                fmt::print(fg(fmt::color::red), "\nfail to open the file!\n");
                fmt::print(fg(fmt::color::red), "please check the input file's path\n");
                fmt::print(fg(fmt::color::red), "--end--\n");
                exit(1);
            }
            c = file.get();
        };
        ~scanning() {
            file.close();
        };

        scanning(scanning const &)      = delete;
        scanning(scanning&&)            = delete;
        scanning& operator=(scanning&)  = delete;
        scanning& operator=(scanning&&) = delete;

        //-----------------------------------------------------------------------------------
        //
        //-----------------------------------------------------------------------------------

        std::tuple<token, std::string> next_token();
        token                          get_token(std::tuple<token, std::string> &tuple_);
        std::string                    get_value(std::tuple<token, std::string> &tuple_);

        //测试词义分析器函数
        void token_output() {
            while (!file.eof()) {
                auto &&a = next_token();
                if (test_output_token.find(std::get<0>(a)) == test_output_token.end())
                    fmt::print("[token:] indentif [string:] \"{}\"\n", std::get<1>(a));
                else
                    fmt::print("[token:] {:8} [string:] \"{}\"\n", test_output_token.at(std::get<0>(a)), std::get<1>(a));
            }
        }
};
