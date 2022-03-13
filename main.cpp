#include "./code/parser.cpp"
#include "fmt/core.h"

//run mode
//for debug
enum class mode : char {
    lexical_analysis_output = 's',
    yes   = 'y',

    quit  = 'q',
    help  = 'h',
    debug = 'd',
};

//-----------------------------------------------------------------------------------------------
//dec
//-----------------------------------------------------------------------------------------------

void choose_mode_run();
void output_help_information();

//-----------------------------------------------------------------------------------------------
//main func
//-----------------------------------------------------------------------------------------------
//int main(int argc, char* argv[]) {

std::string file_path = "/home/lzj/code/program/script/test/i1";
std::string output_file_path = "/home/lzj/code/program/script/test/o1";

int main() {
    //cin.tie(0);
    //std::ios::sync_with_stdio(false);
    fmt::print("path of input file  : ");
    //std::cin >> file_path;
    fmt::print("path of output file : ");
    //std::cin >> output_file_path;
    choose_mode_run();
    return 0;
}

//-----------------------------------------------------------------------------------------------
//def
//-----------------------------------------------------------------------------------------------
inline void choose_mode_run() {
    output_help_information();
    mode choose_mode;
    char in;
    while (choose_mode != mode::quit) {
        fmt::print("(help -h)enter to continue: ");
        std::cin >> in;
        choose_mode = static_cast<mode>(in);
        switch (choose_mode) {
            case mode::lexical_analysis_output : {
                scanning s{file_path};
                s.token_output();
                break;
            }
            case mode::yes : {
                parser p{file_path, output_file_path};
                break;
            }
            case mode::help : {
                output_help_information();
            }
            default:
                break;
        }
    }
    return;
}

inline void output_help_information() {
    fmt::print("\n");
    fmt::print("(s) scan    --lexical analysis and output the token\n");
    fmt::print("(y) yes     --get the asm file");
    fmt::print("(b)\n");
    fmt::print("(c) create  --ctreate the\n");
    fmt::print("(h) help\n");
    fmt::print("(q) quit\n");
    return;
}