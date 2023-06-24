//
// Created by lzj on 2023.6.14.
//

#ifndef TOYC_MODE_H
#define TOYC_MODE_H

#include "mode_input.h"
namespace toy_c::controller
{

extern void mode_scanner();
extern void mode_ast();
extern void mode_specific_tree();

}  // namespace toy_c::controller

#endif  // TOYC_MODE_H
