#ifndef INNER_CPP
#define INNER_CPP

#include "scanning.h"
#include <iostream>
#include <string>

#define TODO                                                                             \
  fmt::print(                                                                            \
    fg(fmt::color::red), "haven't finished part at :{}:{}\n", __FILE__, __LINE__         \
  );                                                                                     \
  exit(0);

#define PRINT_TOKEN_IN_SCAN                                                              \
  fmt::print(                                                                            \
    fg(fmt::color::red), "the token is:{} line:{} column:{}\n",                          \
    scan.get_current_value(), scan.get_line(), scan.get_column()                         \
  );

#define PRINT_TOKEN(token)                                                               \
  fmt::print(fg(fmt::color::red), "the token is:{}\n", static_cast<unsigned char>(token));

#define PRINT_NODE_TYPE(node_type)                                                       \
  fmt::print(                                                                            \
    fg(fmt::color::red), "the node type is:{}\n", static_cast<unsigned char>(node_type)  \
  );

#define SWITCH_ERROR                                                                     \
  fmt::print(                                                                            \
    fg(fmt::color::red),                                                                 \
    "a not expected switch case happen in:  {}:{}\nin function : {}()\n", __FILE__,      \
    __LINE__, __FUNCTION__                                                               \
  );                                                                                     \
  exit(0);

#define NOT_REACHABLE                                                                    \
  fmt::print(                                                                            \
    fg(fmt::color::red), "The program cannot be executed here:{}:{}\n", __FILE__,        \
    __LINE__                                                                             \
  );                                                                                     \
  exit(0);

#define NODE_TYPE_WRONG(type1, type2)                                                    \
  if (type1 != type2)                                                                    \
  {                                                                                      \
    fmt::print(fg(fmt::color::red), "node type is wrong at: {}:{}", __FILE__, __LINE__); \
    exit(0);                                                                             \
  }

#define FILE_FAIL_OPEN                                                                   \
  fmt::print(                                                                            \
    fg(fmt::color::red), "file fail to open at :{}:{}\nin: function {}\n", __FILE__,     \
    __LINE__, __FUNCTON__                                                                \
  );                                                                                     \
  exit(0);


#define PRINT_SPEC_TREE_SIZE                                                             \
  fmt::print("tree size : {}     {}:{}", ptr_tree_body->size(), __FILE__, __LINE__);     \
  std::cout << std::endl;

#endif