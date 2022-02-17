//#ifndef PARSER
//#define PARSER
#include "./parser.cpp"
//#endif
enum class mode : char {
    lexical_analysis_output = 'a',

    quit = 'q',
    help = 'h',
    debug
};
void choose_mode_run(parser& p);
void output_help();

//int main(int argc, char* argv[]) {
int main() {
    std::ios::sync_with_stdio(false);
    std::cout << "path of MyScript(.txt) file: ";
    std::string file_path;
    std::cin >> file_path;
    parser p(file_path);
    choose_mode_run(p);
    return 0;
}

void choose_mode_run(parser& p) {
    output_help();
    mode choose_mode;
    char in;
    while (choose_mode != mode::quit) {
        std::cout << "(help -h)enter to continue: ";
        std::cin >> in;
        choose_mode = static_cast<mode>(in);
        switch (choose_mode) {
            case mode::lexical_analysis_output :
                p.test_output();
                break;
            case mode::help :
                output_help();
            default:
                break;
        }
    }
    return;
}

void output_help() {
    std::cout
    << "-a lexical analysis output" << std::endl
    << "-b " << std::endl
    << "-c " << std::endl
    << "-h help" << std::endl
    << "-q quit" << std::endl;
    return;
}