#ifndef FSTREAM_GUARD_H
#define FSTREAM_GUARD_H

#pragma once
#include <fstream>

namespace toy_c {

enum mode {
  read,
  wirte,
};

class fstream_guard {
private:
  std::fstream fstream;

public:
  explicit fstream_guard(std::string file_path, mode mode);
  fstream_guard();
  ~fstream_guard();
  fstream_guard(fstream_guard const &)       = delete;
  fstream_guard(fstream_guard &&)            = delete;
  fstream_guard &operator=(fstream_guard &)  = delete;
  fstream_guard &operator=(fstream_guard &&) = delete;

  void          open(std::string input_file_path, mode mode);
  bool          eof();
  std::fstream &operator<<(std::string);
  char          get();
};

}  // namespace toy_c


#endif