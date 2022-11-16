#include "build_llvm_ir.h"
#include "fmt/core.h"
#include "parser.h"
#include "spec_tree.h"
#include "spec_tree_node.h"
#include <cstddef>
#include <gflags/gflags.h>
#include <map>
#include <string>

//#define NDEBUG

enum class mode : char
{
  scan      = 's',
  yes       = 'y',
  mid       = 'm',
  quit      = 'q',
  help      = 'h',
  debug     = 'd',
  tree      = 't',
  llvm_ir   = 'l',
  llvm_opt  = 'o',
  spec_tree = 'S'
};

//
DEFINE_string(i, "", "input file path");
DEFINE_string(o, "", "output file path");
DEFINE_string(m, "", "mode");


void choose_mode_run();
void output_help_information();
void gflags_choose_mode_run();

// main func

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    fmt::print("path of input file  : ");
#ifndef NDEBUG
    input_file_path = "/home/lzj/Code/Cpp/ToyC/debug/input.c";
#endif
#ifdef NDEBUG
    std::cin >> input_file_path;
#endif
    fmt::print("path of output file : ");
#ifndef NDEBUG
    output_file_path = "/home/lzj/Code/Cpp/ToyC/debug/output";
#endif
#ifdef NDEBUG
    std::cin >> output_file_path;
#endif
    choose_mode_run();
  }
  else
  {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    gflags_choose_mode_run();
  }
  fmt::print("FINISH\n");
  return 0;
}

void gflags_choose_mode_run()
{
  const std::map<std::string, mode> str_to_char_mode{
    {"scan",  mode::scan     },
    {"yes",   mode::yes      },
    {"mid",   mode::mid      },
    {"quit",  mode::quit     },
    {"help",  mode::help     },
    {"debug", mode::debug    },
    {"tree",  mode::tree     },
    {"ir",    mode::llvm_ir  },
    {"opt",   mode::llvm_opt },
    {"spt",   mode::spec_tree}
  };

  input_file_path  = FLAGS_i;
  output_file_path = FLAGS_o;

  if (str_to_char_mode.find(FLAGS_m) == str_to_char_mode.end())
  {
    fmt::print(fg(fmt::color::red), "unknow mode : {}\n", FLAGS_m);
    exit(0);
  }

  switch (str_to_char_mode.at(FLAGS_m))
  {
    case mode::scan: {
      toy_c::scanning s(input_file_path);
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
      // toy_c::build_llvm_ir ir;
      // ir.output_llvm_ir();
      spt::Tree spt_tree;
      spt_tree.generate_llvm_ir();
      break;
    }
    case mode::llvm_opt: {
      int done = system("opt -dot-cfg -S ./../test/debug/1");
      if (done == 127)
      {
        fmt::print("done");
      }
      if (done == -1)
      {
        fmt::print("no");
      }
      break;
    }
    case mode::spec_tree: {
      spt::Tree spt_tree;
      spt_tree.print_spec_tree();
      break;
    }
    default:
      fmt::print("unknown mode");
  }
}

// def

void choose_mode_run()
{
  output_help_information();
  mode choose_mode = mode::help;
  char in;
  while (choose_mode != mode::quit)
  {
    fmt::print("(help -h)enter to continue: ");
    std::cin >> in;
    choose_mode = static_cast<mode>(in);
    switch (choose_mode)
    {
      case mode::scan: {
        toy_c::scanning s(input_file_path);
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
        // toy_c::build_llvm_ir ir;
        // ir.output_llvm_ir();
        spt::Tree spt_tree;
        spt_tree.generate_llvm_ir();
        break;
      }
      case mode::llvm_opt: {
        int done = system("opt -dot-cfg -S ./../test/debug/1");
        if (done == 127)
        {
          fmt::print("done");
        }
        if (done == -1)
        {
          fmt::print("no");
        }
        break;
      }
      case mode::spec_tree: {
        spt::Tree spt_tree;
        spt_tree.print_spec_tree();
        break;
      }
      default:
        fmt::print("unknown mode");
    }
  }
  return;
}

void output_help_information()
{
  fmt::print("\n _______           _____ \n");
  fmt::print("|__   __|         / ____|\n");
  fmt::print("   | | ___  _   _| |     \n");
  fmt::print("   | |/ _ \\| | | | |     \n");
  fmt::print("   | | (_) | |_| | |____ \n");
  fmt::print("   |_|\\___/ \\__, |\\_____|\n");
  fmt::print("             __/ |       \n");
  fmt::print("            |___/        \n");
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
  fmt::print("-S  spec_tree\n");
  fmt::print("-q  quit         quit\n");
  return;
}
