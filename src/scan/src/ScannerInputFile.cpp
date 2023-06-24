//
// Created by lzj on 2023.6.4.
//

#include "scan/ScannerInputFile.h"
#include "controller/io_file.h"
#include <fmt/color.h>
#include <fmt/core.h>

namespace toy_c::scan
{

void ScannerInputFile::judge_char()
{
  if (m_currentChar == '\n')
  {
    ++m_currentCharLine;
    m_currentCharColumn = 0;
  }
  else
  {
    ++m_currentCharColumn;
  }
}


/**
 *
 */
void ScannerInputFile::openFile()
{
  m_inputFile.open(controller::absoluteInputFilePath);
  if (!m_inputFile.is_open())
  {
    fmt::print(
      fg(fmt::color::red), "can't open file at: {}\n",
      controller::absoluteInputFilePath.string()
    );
    exit(0);
  }
  nextChar();
}
/**
 *
 */
void ScannerInputFile::nextChar()
{
  m_inputFile.get(m_currentChar);
  judge_char();
}
/**
 *
 * @return
 */
char ScannerInputFile::getCurrentChar() const
{
  return m_currentChar;
}

/**
 *
 * @return
 */
size_t ScannerInputFile::getCurrentLine() const
{
  return m_currentCharLine;
}

/**
 *
 * @return
 */
size_t ScannerInputFile::getCurrentColumn() const
{
  return m_currentCharColumn;
}

/**
 *
 * @return
 */
bool ScannerInputFile::isEnd()
{
  return m_inputFile.eof();
}

/**
 *
 * @return
 */
bool ScannerInputFile::currentCharIsUseless() const
{
  static char const useless_char[] = {'\n', '\r', '\t', ' '};
  for (auto i : useless_char)
  {
    if (i == m_currentChar)
    {
      return true;
    }
  }
  return false;
}

void ScannerInputFile::eatCurrentChar(char c_)
{
  if (m_currentChar != c_)
  {
    fmt::print(fg(fmt::color::red), "expected char: {} but: {}\n", c_, m_currentChar);
    fmt::print(fg(fmt::color::red), "at {}:{}\n", m_currentCharLine, m_currentCharColumn);
    exit(1);
  }
  nextChar();
}

}  // namespace toy_c::scan