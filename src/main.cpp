#include "fmt/color.h"
#define __TOYC_MACRO_DEBUGE_MODE__

#include "controller/mode_input.h"
#include <fmt/core.h>
#include <gflags/gflags.h>

namespace toy_c
{

}


int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    toy_c::controller::no_input_mode();
  }
  else
  {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    toy_c::controller::had_input_mode();
  }
  fmt::print(fmt::fg(fmt::color::green), "\n -- FINISH! --\n");
  return 0;
}
