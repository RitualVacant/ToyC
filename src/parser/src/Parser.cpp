//
// Created by lzj on 2023.6.5.
//
#include "parser/Parser.h"
#include "macro/error.h"


namespace toy_c::parser
{

Parser::Parser() : ParserImpl() {}

/**
 *
 * @return
 */
nlohmann::json Parser::toJson()
{
  nlohmann::json json;
  json["TranslationUnit"] = [&] {
    auto list = nlohmann::json::array();
    for (auto i : m_translationUnit)
    {
      list.push_back(i->toJson());
    }
    return list;
  }();
  return json;
}

/**
 *
 */
void Parser::genSpec() {}

}  // namespace toy_c::parser