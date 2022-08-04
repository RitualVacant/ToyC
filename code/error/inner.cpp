#ifndef INNER_CPP
#define INNER_CPP

#include <string>
#include "scanning.h"

#define TODO \
fmt::print(fg(fmt::color::red), "haven't finshed part at :{}:{}\n", __FILE__, __LINE__);\
exit(0);

#define PRINT_TOKEN \
fmt::print(fg(fmt::color::red), "the token is:{} line:{} column:{}\n", scan.get_current_value(), line, column);

#define SWITCH_ERROR \
fmt::print(fg(fmt::color::red), "a not expected switch case happen in:  {}:{}\nin function : {}()\n", __FILE__,  __LINE__, __FUNCTION__);\
exit(0);

#define NOT_EXCUTABLE \
fmt::print(fg(fmt::color::red), "The program cannot be executed here:{}:{}\n", __FILE__, __LINE__);\
exit(0);

#define NOTD_TYPE_WRONG(type1, type2) \
if (type1 != type2) {\
    fmt::print(fg(fmt::color::red), "node type is wrong at: {}:{}", __FILE__, __LINE__);\
    exit(0);\
}

#define FILE_FAIL_OPEN \
fmt::print(fg(fmt::color::red),"file fail to open at :{}:{}\nin: funcion {}\n", __FILE__, __LINE__, __FUNCTON__);\
exit(0);


#endif