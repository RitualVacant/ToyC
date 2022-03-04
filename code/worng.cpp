#ifndef FMT_CORE_H
#define FMT_CORE_H
#include "fmt/core.h"
#endif

namespace worng {

    //this funcion's argc will rewirte after
    //need really line and colume
    void print_worng_redef(std::size_t line = 0, std::size_t colume = 0) {
        fmt::print("a worng at (line : {} , colume : {}");
        fmt::print("you define twice!");
        exit(0);
        return;
    }
}