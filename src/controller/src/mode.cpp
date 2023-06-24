//
// Created by lzj on 2023.6.14.
//

#include "controller/io_file.h"
#include "macro/error.h"
#include "parser/Parser.h"
#include "parser/ParserImpl.h"
#include "parser/parser.h"
#include "scan/Scanner.h"
#include <fmt/color.h>
#include <fmt/core.h>

namespace toy_c::controller
{

/**
 * @brief
 */
void mode_scanner()
{
  std::ofstream outputFile(absoluteOutputFilePath, std::ios::out | std::ios::trunc);

  if (!outputFile.is_open())
  {
    fmt::print(
      fg(fmt::color::red), "can't open file at: {}\n",
      std::filesystem::absolute(absoluteOutputFilePath).string()
    );
    exit(1);
  }

  scan::Scanner scanner;

  while (!scanner.isCompleted())
  {
    scanner.nextToken();
    auto        current_token = scanner.getCurrentToken();
    std::string tokenType_str;
    if (token::c_TokenType_map_string.contains(current_token.type))
    {
      tokenType_str = token::c_TokenType_map_string.at(scanner.getCurrentTokenType());
    }
    else
    {
      // can't find tokenType_str in map, probably forgot add it to map
      NOT_REACHABLE
    }
    outputFile << fmt::format(
      "token: {:30} string: {:30} line: {:6} column: {}\n", tokenType_str,
      current_token.str, current_token.line, current_token.column
    );
  }
}

/**
 *
 */
void mode_ast()
{
  std::ofstream outputFile(absoluteOutputFilePath, std::ios::out | std::ios::trunc);

  if (!outputFile.is_open())
  {
    fmt::print(
      fg(fmt::color::red), "can't open file at: {}\n",
      std::filesystem::absolute(absoluteOutputFilePath).string()
    );
    exit(1);
  }

  parser::Parser parser;

  nlohmann::json json = parser.toJson();
  outputFile << "@startjson\n";
  outputFile << json.dump();
  outputFile << "\n@endjson";
}

void mode_specific_tree()
{
  parser::Parser parser;
}

}  // namespace toy_c::controller