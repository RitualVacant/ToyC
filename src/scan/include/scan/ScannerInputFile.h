//
// Created by lzj on 2023.6.4.
//

#ifndef TOYC_SCANNERINPUTFILE_H
#define TOYC_SCANNERINPUTFILE_H

#include <fstream>

namespace toy_c::scan
{

class ScannerInputFile
{
public:
  ScannerInputFile()  = default;
  ~ScannerInputFile() = default;

  void openFile();
  char getCurrentChar() const;
  bool currentCharIsUseless() const;
  void eatCurrentChar(char c_);
  void nextChar();
  bool isEnd();

  size_t getCurrentLine() const;
  size_t getCurrentColumn() const;


private:
  char          m_currentChar;
  std::ifstream m_inputFile;
  size_t        m_currentCharLine   = 1;
  size_t        m_currentCharColumn = 0;

  void judge_char();
};

}  // namespace toy_c::scan


#endif  // TOYC_SCANNERINPUTFILE_H
