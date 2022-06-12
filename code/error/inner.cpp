#ifndef INNER_CPP
#define INNER_CPP

#include <string>
#include "./../scanning.cpp"
#define print_token  fmt::print(fg(fmt::color::red) ,"the token is:{}\n", scan.get_current_value());
#define switch_error fmt::print(fg(fmt::color::red) ,"a not expected switch case happen in:  {}:{}\nin function : {}()\n", __FILE__,  __LINE__, __FUNCTION__);\
                     exit(0);

#endif