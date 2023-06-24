//
// Created by lzj on 2023.6.4.
//

#include "scan/Scanner.h"
#include "controller/io_file.h"
#include "macro/error.h"
#include "scan/judge_char.h"
#include "token/Token.h"
#include "token/TokenType.h"

namespace toy_c::scan
{

Scanner::Scanner()
{
  m_inputFile.openFile();
}

/**
 *
 */
void Scanner::nextToken()
{
  if (!m_isReallyNextToken)
  {
    m_isReallyNextToken = true;
    m_currentToken      = m_nextToken;
    return;
  }

  while (m_inputFile.currentCharIsUseless())
  {
    m_inputFile.nextChar();
    if (m_inputFile.isEnd())
    {
      m_currentToken.type = token::TokenType::eof;
      m_currentToken.str  = "eof";
      return;
    }
  }

  m_currentToken.line   = m_inputFile.getCurrentLine();
  m_currentToken.column = m_inputFile.getCurrentColumn();

  switch (m_inputFile.getCurrentChar())
  {
    case '?': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::question_mark;
      m_currentToken.str  = "?";
      break;
    }

    case '[': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::l_mid_par;
      m_currentToken.str  = "[";
      break;
    }

    case ']': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::r_mid_par;
      m_currentToken.str  = "]";
      break;
    }

    case '{': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::l_big_par;
      m_currentToken.str  = "{";
      break;
    }

    case '}': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::r_big_par;
      m_currentToken.str  = "}";
      break;
    }

    case '(': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::l_par;
      m_currentToken.str  = "(";
      break;
    }

    case ')': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::r_par;
      m_currentToken.str  = ")";
      break;
    }

    case ',': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::comma;
      m_currentToken.str  = ",";
      break;
    }

    case ';': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::semicolon;
      m_currentToken.str  = ";";
      break;
    }

    case ':': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::colon;
      m_currentToken.str  = ":";
      break;
    }

    case '~': {
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::bit_not;
      m_currentToken.str  = "~";
      break;
    }

    case '+': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::plus_agn;
        m_currentToken.str  = "+=";
      }
      else if (m_inputFile.getCurrentChar() == '+')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::self_plus;
        m_currentToken.str  = "++";
      }
      else
      {
        m_currentToken.type = token::TokenType::plus;
        m_currentToken.str  = "+";
      }
      break;
    }

    case '-': {
      // -=
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::minus_agn;
        m_currentToken.str  = "-=";
      }
      // --
      else if (m_inputFile.getCurrentChar() == '-')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::self_minus;
        m_currentToken.str  = "--";
      }
      // ->
      else if (m_inputFile.getCurrentChar() == '>')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::arrow;
        m_currentToken.str  = "->";
      }
      // negative number
      else if (is_digit(m_inputFile.getCurrentChar()))
      {
        auto line             = m_currentToken.line;
        auto column           = m_currentToken.column;
        m_currentToken        = to_number(true);
        m_currentToken.line   = line;
        m_currentToken.column = column;
      }
      // signal -
      else
      {
        m_currentToken.type = token::TokenType::minus;
        m_currentToken.str  = "-";
      }
      break;
    }

    case '*': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::times_agn;
        m_currentToken.str  = "*=";
      }
      else
      {
        m_currentToken.type = token::TokenType::times;
        m_currentToken.str  = "*";
      }
      break;
    }

    case '/': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::div_agn;
        m_currentToken.str  = "/=";
      }
      else
      {
        m_currentToken.type = token::TokenType::div;
        m_currentToken.str  = "/";
      }
      break;
    }

    case '%': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::mod_agn;
        m_currentToken.str  = "%=";
      }
      else
      {
        m_currentToken.type = token::TokenType::div;
        m_currentToken.str  = "%";
      }
      break;
    }

    case '=': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::equ;
        m_currentToken.str  = "==";
      }
      else
      {
        m_currentToken.type = token::TokenType::assign;
        m_currentToken.str  = "=";
      }
      break;
    }

    case '!': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::not_equ;
        m_currentToken.str  = "!=";
      }
      else
      {
        m_currentToken.type = token::TokenType::log_not;
        m_currentToken.str  = "!";
      }
      break;
    }

    case '>': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::great_equ;
        m_currentToken.str  = ">=";
      }
      else if (m_inputFile.getCurrentChar() == '>')
      {
        m_inputFile.nextChar();
        if (m_inputFile.getCurrentChar() == '=')
        {
          m_inputFile.nextChar();
          m_currentToken.type = token::TokenType::r_shift_agn;
          m_currentToken.str  = ">>=";
        }
        else
        {
          m_currentToken.type = token::TokenType::r_shift;
          m_currentToken.str  = ">>";
        }
      }
      else
      {
        m_currentToken.type = token::TokenType::great;
        m_currentToken.str  = ">";
      }
      break;
    }

    case '<': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::less_equ;
        m_currentToken.str  = "<=";
      }
      else if (m_inputFile.getCurrentChar() == '<')
      {
        m_inputFile.nextChar();
        if (m_inputFile.getCurrentChar() == '=')
        {
          m_inputFile.nextChar();
          m_currentToken.type = token::TokenType::l_shift_agn;
          m_currentToken.str  = "<<=";
        }
        else
        {
          m_currentToken.type = token::TokenType::l_shift;
          m_currentToken.str  = "<<";
        }
      }
      else
      {
        m_currentToken.type = token::TokenType::less;
        m_currentToken.str  = "<";
      }
      break;
    }

    case '&': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '&')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::log_and;
        m_currentToken.str  = "&&";
      }
      else if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::bit_and_agn;
        m_currentToken.str  = "&=";
      }
      else
      {
        m_currentToken.type = token::TokenType::bit_and;
        m_currentToken.str  = "&";
      }
      break;
    }

    case '|': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '|')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::log_or;
        m_currentToken.str  = "||";
      }
      else if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::bit_or_agn;
        m_currentToken.str  = "|=";
      }
      else
      {
        m_currentToken.type = token::TokenType::bit_or;
        m_currentToken.str  = "|";
      }
      break;
    }

    case '^': {
      m_inputFile.nextChar();
      if (m_inputFile.getCurrentChar() == '=')
      {
        m_inputFile.nextChar();
        m_currentToken.type = token::TokenType::bit_xor_agn;
        m_currentToken.str  = "^=";
      }
      else
      {
        m_currentToken.type = token::TokenType::bit_xor;
        m_currentToken.str  = "^";
      }
      break;
    }
    case '\'': {
      m_inputFile.eatCurrentChar('\'');
      if (m_inputFile.getCurrentChar() == '\\')
      {
        m_inputFile.nextChar();
        TODO
      }
      else
      {
        m_currentToken.type = token::TokenType::literal_char;
        m_currentToken.str  = m_inputFile.getCurrentChar();
        m_inputFile.nextChar();
        m_inputFile.eatCurrentChar('\'');
      }
      break;
    }

    case '"': {
      m_inputFile.nextChar();
      std::string str;
      while (m_inputFile.getCurrentChar() != '"')
      {
        if (m_inputFile.getCurrentChar() == '/')
        {
          m_inputFile.nextChar();
          switch (m_inputFile.getCurrentChar())
          {
            case '"':
              str += '"';
            case '\'':
              str += '\'';
            default: {
              UNEXPECTED_SWITCH_CASE
            }
          }
        }
        else
        {
          str += m_inputFile.getCurrentChar();
          m_inputFile.nextChar();
        }
      }
      // eat "
      m_inputFile.nextChar();
      m_currentToken.type = token::TokenType::literal_string;
      m_currentToken.str  = str;
      break;
    }

    default: {
      auto line   = m_currentToken.line;
      auto column = m_currentToken.column;
      if (is_digit(m_inputFile.getCurrentChar()))
      {
        m_currentToken = to_number(false);
      }
      else if (is_alpha(m_inputFile.getCurrentChar()))
      {
        m_currentToken = to_keyWord_or_identify();
      }
      m_currentToken.line   = line;
      m_currentToken.column = column;
      break;
    }
  }
}

/**
 *
 * @param is_negative_
 * @return
 */
token::Token Scanner::to_number(bool is_negative_)
{
  std::string str;
  bool        is_float = false;

  str += m_inputFile.getCurrentChar();
  m_inputFile.nextChar();

  while (is_digit(m_inputFile.getCurrentChar()) || m_inputFile.getCurrentChar() == '.')
  {
    if (m_inputFile.getCurrentChar() == '.')
    {
      is_float = true;
    }
    str += m_inputFile.getCurrentChar();
    m_inputFile.nextChar();
  }
  if (is_float)
  {
    if (is_negative_)
    {
      return token::Token{token::TokenType::literal_negative_float, str};
    }
    else
    {
      return token::Token{token::TokenType::literal_positive_float, str};
    }
  }
  else
  {
    if (is_negative_)
    {
      return token::Token{token::TokenType::literal_negative_integer, str};
    }
    else
    {
      return token::Token{token::TokenType::literal_positive_integer, str};
    }
  }
}

/**
 *
 * @return
 */
token::Token Scanner::to_keyWord_or_identify()
{
  std::string str;

  str += m_inputFile.getCurrentChar();
  m_inputFile.nextChar();

  while (is_digit(m_inputFile.getCurrentChar()) || is_alpha(m_inputFile.getCurrentChar())
         || m_inputFile.getCurrentChar() == '_')
  {
    str += m_inputFile.getCurrentChar();
    m_inputFile.nextChar();
  }

  if (token::c_keywords_map_TokenType.find(str) == token::c_keywords_map_TokenType.end())
  {
    return token::Token{token::TokenType::identify, str};
  }
  else
  {
    return token::Token{token::c_keywords_map_TokenType.at(str), str};
  }
  NOT_REACHABLE
}

/**
 *
 * @return
 */
bool Scanner::isCompleted()
{
  return m_inputFile.isEnd();
}

/**
 *
 * @return
 */
token::Token Scanner::getCurrentToken()
{
  return m_currentToken;
}

/**
 *
 * @return
 */
token::TokenType Scanner::getCurrentTokenType()
{
  return m_currentToken.type;
}
/**
 *
 * @return
 */
std::string Scanner::getCurrentTokenStr()
{
  return m_currentToken.str;
}


void Scanner::eatCurrentToken(token::TokenType token_type_)
{
  if (m_currentToken.type != token_type_)
  {
    fmt::print(
      fg(fmt::color::red), "[ERROR] {}:{} {}\n",
      controller::absoluteInputFilePath.string(), m_currentToken.line,
      m_currentToken.column
    );
    fmt::print(
      fg(fmt::color::red), "should be {} but {}",
      token::c_TokenType_map_string.at(m_currentToken.type),
      token::c_TokenType_map_string.at(token_type_)
    );
    exit(0);
  }
  nextToken();
}

/**
 *
 * @return
 */
token::Token Scanner::getNextToken()
{
  auto currentToken = m_currentToken;
  nextToken();
  auto nextToken = m_currentToken;

  m_currentToken      = currentToken;
  m_nextToken         = nextToken;
  m_isReallyNextToken = false;

  return m_nextToken;
}

/**
 *
 * @return
 */
token::TokenType Scanner::getNextTokenType()
{
  getNextToken();
  return m_nextToken.type;
}

/**
 *
 * @return
 */
std::string Scanner::getNextTokenStr()
{
  getNextToken();
  return m_nextToken.str;
}

}  // namespace toy_c::scan
