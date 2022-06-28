#include <string>

std::string file_path = "/home/lzj/code/program/script/test/i1";
std::string output_file_path = "/home/lzj/code/program/script/test/o1";

#pragma once
#include "./code/class/interface/parser.h"
#include "./code/class/interface/build_llvm_ir.h"
#include "fmt/core.h"

enum class mode : char {
    scan  = 's',
    yes   = 'y',
    mid   = 'm',
    quit  = 'q',
    help  = 'h',
    debug = 'd',
    tree  = 't',
    llvm_ir = 'l'
};


//-----------------------------------------------------------------------------------------------
//dec
//-----------------------------------------------------------------------------------------------

void choose_mode_run();
void output_help_information();

//-----------------------------------------------------------------------------------------------
//main func
//-----------------------------------------------------------------------------------------------
std::vector<std::string> command;
int main(int argc, char* argv[]) {
    if (argc == 1) {
        fmt::print("path of input file  : ");
        //std::cin >> file_path;
        fmt::print("path of output file : ");
        //std::cin >> output_file_path;
        choose_mode_run();
    }
    else {
        file_path = argv[1];
        output_file_path = argv[2];
        parser p;
        fmt::print("\nMyScript {} {}\nDone\n", file_path, output_file_path);
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------
//def
//-----------------------------------------------------------------------------------------------
void parser_command() {

}

void choose_mode_run() {
    output_help_information();
    mode choose_mode = mode::help;
    char in;
    while (choose_mode != mode::quit) {
        fmt::print("(help -h)enter to continue: ");
        std::cin >> in;
        choose_mode = static_cast<mode>(in);
        switch (choose_mode) {
            case mode::scan: {
                scanning s(file_path);
                s.token_output();
                break;
            }
            case mode::tree: {
                parser p;
                p.print_synctax_tree();
                break;
            }
            case mode::mid: {
                parser p;
                //p.print_mid_code();
                break;
            }
            case mode::yes: {
                parser p;
                break;
            }
            case mode::help: {
                output_help_information();
                break;
            }
            case mode::llvm_ir: {
                build_llvm_ir ir;
                ir.output_llvm_ir();
                break;
            }
            default:
                break;
        }
    }
    return;
}

void output_help_information() {
    fmt::print("\n");
    fmt::print("size of node: {}B\n", sizeof(ast::node));
    fmt::print("size of node.value: {}B\n", sizeof(ast::node::value));
    fmt::print("size of node.next: {}B\n", sizeof(ast::node::next));
    fmt::print("(s) scan         --lexical analysis and output the token\n");
    fmt::print("(y) yes          --get the asm_code file\n");
    fmt::print("(m) mid_code     --get transfrom code\n");
    fmt::print("(t) tree         --print synctax tree\n");
    fmt::print("(b) \n");
    fmt::print("(c) create       --ctreate the\n");
    fmt::print("(h) help         --print table\n");
    fmt::print("(l) llvm         --output llvm ir\n");
    fmt::print("(q) quit         --quit\n");
    return;
}
