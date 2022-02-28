#ifndef PARSER_CPP
#define PARSER_CPP
#include "./code/parser.cpp"
#endif

//run mode
//for debug
enum class mode : char {
    lexical_analysis_output = 's',

    quit = 'q',
    help = 'h',
    debug
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

std::string file_path;
std::string output_path;

int main() {
    //std::ios::sync_with_stdio(false);
    fmt::print("path of MyScript(.txt) file: ");
    std::cin >> file_path;
    fmt::print("path of output file path: ");
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
                scanning s(file_path);
                s.token_output();
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
    fmt::print("(s) scan--lexical analysis and output the token\n");
    fmt::print("(b)\n");
    fmt::print("(c)\n");
    fmt::print("(h) help\n");
    fmt::print("(q) quit\n");
    return;
}