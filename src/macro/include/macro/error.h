//
// Created by lzj on 2023.6.4.
//

#ifndef TOYC_ERROR_H
#define TOYC_ERROR_H

#include <fmt/color.h>
#include <fmt/core.h>
#include <thread>

#define UNEXPECTED_SWITCH_CASE                                                           \
  fmt::print(                                                                            \
    fg(fmt::color::yellow_green),                                                        \
    "a not expected switch case happen in:  {}:{}\nin function : {}()\n", __FILE__,      \
    __LINE__, __FUNCTION__                                                               \
  );                                                                                     \
  exit(1);

#define TODO                                                                             \
  fmt::print(                                                                            \
    fg(fmt::color::yellow_green), "haven't finished part at: {}:{}\n", __FILE__,         \
    __LINE__                                                                             \
  );                                                                                     \
  exit(1);

#define NOT_REACHABLE                                                                    \
  fmt::print(                                                                            \
    fg(fmt::color::yellow_green), "The program cannot reach here: {}:{}\n", __FILE__,    \
    __LINE__                                                                             \
  );                                                                                     \
  exit(1);

#endif  // TOYC_ERROR_H
