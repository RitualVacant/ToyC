//
// Created by lzj on 2023.6.3.
//
#include "controller/mode_input.h"
#include "controller/io_file.h"
#include "controller/mode.h"
#include "macro/debug_mode_macro.h"
#include "macro/error.h"
#include <iostream>
#include <nlohmann/json.hpp>


namespace toy_c::controller
{

DEFINE_string(input, "", "input file path");
DEFINE_string(output, "", "output file path");
DEFINE_string(mode, "", "mode");

#define CHAR_SCANNER 's'
#define CHAR_AST_TREE 'a'
#define CHAR_LLVM_IR 'i'
#define CHAR_LLVM_OPT 'o'
#define CHAR_SPEC_TREE 't'
#define CHAR_QUIT 'q'
#define CHAR_HELP 'h'

#define STR_SCANNER "scanner"
#define STR_AST_TREE "ast"

/**
 *
 */
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
  fmt::print("s  scanner      lexical analysis and output the token\n");
  fmt::print("h  help         toJson table\n");
  fmt::print("l  llvm         output llvm ir\n");
  fmt::print("o  dot          dot\n");
  fmt::print("a  ast_tree     ast tree\n");
  fmt::print("t  spec    spec tree\n");
  fmt::print("r  run_test     run test\n");
  fmt::print("q  quit         quit\n");
}

void switch_mode(char mode_char_)
{
  switch (mode_char_)
  {
    case CHAR_QUIT:
      break;
    case CHAR_SCANNER:
      mode_scanner();
      break;
    case CHAR_AST_TREE:
      mode_ast();
      break;
    default:
      fmt::print(fg(fmt::color::yellow), "unexpected command\n");
  }
}

/**
 * @brief no arguments mode
 */
void no_input_mode()
{
  // debug mode
#ifdef __TOYC_MACRO_DEBUGE_MODE__
  toy_c::controller::absoluteInputFilePath  = "../debug/input.c";
  toy_c::controller::absoluteOutputFilePath = "../debug/output";
#endif

  output_help_information();
  char input_char = ' ';

  while (input_char != 'q')
  {
    std::cout << "enter command: ";
    std::cin >> input_char;

    switch_mode(input_char);
  }
}

/**
 *
 */
void had_input_mode()
{
  toy_c::controller::absoluteInputFilePath  = FLAGS_input;
  toy_c::controller::absoluteOutputFilePath = FLAGS_output;

  static std::map<std::string, char> const mode_map = {
    {STR_SCANNER,  CHAR_SCANNER },
    {STR_AST_TREE, CHAR_AST_TREE},
  };

  auto mode = FLAGS_mode;

  if (mode_map.contains(mode))
  {
    switch_mode(mode_map.at(mode));
  }
  else
  {
    fmt::print(fg(fmt::color::yellow), "unexpected command\n");
  }
}

}  // namespace toy_c::controller