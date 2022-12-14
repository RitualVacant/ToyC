#ifndef FSTREAM_GUARD_CPP
#define FSTREAM_GUARD_CPP

#include "fstream_guard.h"
#include <fmt/color.h>
#include <fmt/core.h>

namespace toy_c
{

fstream_guard::fstream_guard(std::string input_file_path, mode mode)
{
  open(input_file_path, mode);
}

fstream_guard::fstream_guard() {}

fstream_guard::~fstream_guard()
{
  fstream.close();
}

void fstream_guard::open(std::string file_path, mode mode)
{
  if (mode == mode::read)
  {
    fstream.open(file_path);
  }
  else
  {
    fstream.open(file_path, std::fstream::out);
  }
  if (!fstream.is_open())
  {
    fmt::print(fg(fmt::color(fmt::color::red)), "can't open file at {}\n", file_path);
    fmt::print(fg(fmt::color(fmt::color::red)), "please check the path\n");
    exit(0);
  }
}

bool fstream_guard::eof()
{
  return fstream.eof();
}

std::fstream &fstream_guard::operator<<(std::string str)
{
  fstream << str;
  return fstream;
}

char fstream_guard::get()
{
  return fstream.get();
}

bool fstream_guard::is_open()
{
  return fstream.is_open();
}

}  // namespace toy_c
#endif
