#ifndef INNER_CPP
#define INNER_CPP

#include <string>
#include "scanning.h"

#define print_token \
fmt::print(fg(fmt::color::red), "the token is:{} line:{} column:{}\n", scan.get_current_value(), line, column);

#define switch_error \
fmt::print(fg(fmt::color::red), "a not expected switch case happen in:  {}:{}\nin function : {}()\n", __FILE__,  __LINE__, __FUNCTION__);\
exit(0);

#define not_excutable \
fmt::print(fg(fmt::color::red), "The program cannot be executed here:{}:{}\n", __FILE__, __LINE__);\
exit(0);

#define node_type_wrong(type1, type2) \
if (type1 != type2) {\
    fmt::print(fg(fmt::color::red), "node type is worong at: {}:{}", __FILE__, __LINE__);\
    exit(0);\
}\

#endif