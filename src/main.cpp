#include "build_llvm_ir.h"
#include "fmt/core.h"
#include "parser.h"
// #include <gperftools/profiler.h>
#include <string>

enum class mode : char {
  scan     = 's',
  yes      = 'y',
  mid      = 'm',
  quit     = 'q',
  help     = 'h',
  debug    = 'd',
  tree     = 't',
  llvm_ir  = 'l',
  llvm_opt = 'o'
};

// dec

void choose_mode_run();
void output_help_information();

// main func
std::vector<std::string> command;

int main(int argc, char *argv[]) {
  // ProfilerStart("ToyC_profiler.prof");

  if (argc == 1) {
    fmt::print("path of input file  : ");
    // std::cin >> file_path;
    fmt::print("path of output file : ");
    // std::cin >> output_file_path;
    choose_mode_run();
  }
  else {
    fmt::print("not support arguments now\n");
  }

  // ProfilerStop();

  return 0;
}

// def

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
        toy_c::scanning s(file_path);
        s.token_output();
        break;
      }
      case mode::tree: {
        toy_c::parser p;
        p.print_syntax_tree();
        break;
      }
      case mode::mid: {
        toy_c::parser p;
        // p.print_mid_code();
        break;
      }
      case mode::yes: {
        toy_c::parser p;
        break;
      }
      case mode::help: {
        output_help_information();
        break;
      }
      case mode::llvm_ir: {
        toy_c::build_llvm_ir ir;
        ir.output_llvm_ir();
        break;
      }
      case mode::llvm_opt: {
        int done = system("opt -dot-cfg -S ./../test/llvm.ll");
        if (done == 127) {
          fmt::print("done");
        }
        if (done == -1) {
          fmt::print("no");
        }
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
  fmt::print("-s  scan         lexical analysis and output the token\n");
  fmt::print("-y  yes          get the asm_code file\n");
  fmt::print("-m  mid_code     get transform code\n");
  fmt::print("-t  tree         print syntax tree1\n");
  fmt::print("-b  \n");
  fmt::print("-c  create       create the\n");
  fmt::print("-h  help         print table\n");
  fmt::print("-l  llvm         output llvm ir\n");
  fmt::print("-o  dot          dot\n");
  fmt::print("-q  quit         quit\n");
  return;
}
