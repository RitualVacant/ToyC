//
// Created by lzj on 2023.6.3.
//

#include <filesystem>
#include <string>


#ifndef TOYC_IO_FILE_H
#  define TOYC_IO_FILE_H

namespace toy_c::controller
{

inline std::filesystem::path absoluteInputFilePath;
inline std::filesystem::path absoluteOutputFilePath;

}  // namespace toy_c::controller


#endif  // TOYC_IO_FILE_H