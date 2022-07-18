#ifndef FSTREAM_GUARD_CPP
#define FSTREAM_GUARD_CPP

#pragma once
#include "fstream_guard.h"
#include <fmt/core.h>
#include <fmt/color.h>

namespace my {

fstream_guard::fstream_guard(std::string file_path, mode mode) {
    open(file_path, mode);
}

fstream_guard::fstream_guard() {}

fstream_guard::~fstream_guard() {
    fstream.close();
}

void
fstream_guard::open(std::string file_path, mode mode) {
    if (mode == mode::read) {
        fstream.open(file_path);
    }
    else {
        fstream.open(file_path, std::fstream::out);
    }

    if (!fstream.is_open()) {
        fmt::print(fg(fmt::color(fmt::color::red)), "can't open file at {}\n", file_path);
        fmt::print(fg(fmt::color(fmt::color::red)), "please check the path\n");
        exit(0);
    }
}

bool
fstream_guard::eof() {
    return fstream.eof();
}

std::fstream&
fstream_guard::operator<<(std::string str) {
    fstream << str;
    return fstream;
}

char
fstream_guard::get() {
    return fstream.get();
}

}
#endif