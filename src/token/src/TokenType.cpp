//
// Created by lzj on 2023.6.5.
//

#include "token/TokenType.h"
#include <string>
#include <unordered_map>

namespace toy_c::token
{

std::unordered_map<std::string, TokenType> const c_keywords_map_TokenType{
  {"if",         TokenType::key_if        },
  {"else",       TokenType::key_else      },
  {"true",       TokenType::key_true      },
  {"false",      TokenType::key_false     },
  {"while",      TokenType::key_while     },
  {"for",        TokenType::key_for       },
  {"null",       TokenType::key_null      },
  {"return",     TokenType::key_return    },
  {"break",      TokenType::key_break     },
  {"continue",   TokenType::key_continue  },

  {"struct",     TokenType::key_struct    },

  {"int",        TokenType::key_int       },
  {"double",     TokenType::key_double    },
  {"char",       TokenType::key_char      },
  {"long",       TokenType::key_long      },
  {"void",       TokenType::key_void      },
  {"unsigned",   TokenType::key_unsigned  },
  {"signed",     TokenType::key_signed    },
  {"extern",     TokenType::key_extern    },

  {"switch",     TokenType::key_switch    },
  {"do",         TokenType::key_do        },
  {"for",        TokenType::key_for       },
  {"case",       TokenType::key_case      },
  {"default",    TokenType::key_default   },

  {"struct",     TokenType::key_struct    },
  {"auto",       TokenType::key_auto      },
  {"break",      TokenType::key_break     },
  {"case",       TokenType::key_case      },
  {"const",      TokenType::key_const     },
  {"enum",       TokenType::key_enum      },
  {"inline",     TokenType::key_inline    },
  {"register",   TokenType::key_register  },
  {"restrict",   TokenType::key_restrict  },
  {"signed",     TokenType::key_signed    },
  {"short",      TokenType::key_short     },
  {"sizeof",     TokenType::key_sizeof    },
  {"static",     TokenType::key_static    },
  {"switch",     TokenType::key_switch    },
  {"typedef",    TokenType::key_typedef   },
  {"union",      TokenType::key_union     },
  {"void",       TokenType::key_void      },
  {"volatile",   TokenType::key_volatile  },
  {"_Bool",      TokenType::key__Bool     },
  {"Complex",    TokenType::key__Complex  },
  {"_Imaginary", TokenType::key__Imaginary},
};

std::unordered_map<TokenType, std::string> const c_TokenType_map_string{
  {TokenType::literal_string,           "literal_string"          },
  {TokenType::literal_char,             "literal_char"            },
  {TokenType::literal_positive_integer, "literal_positive_integer"},
  {TokenType::literal_negative_integer, "literal_negative_integer"},
  {TokenType::literal_positive_float,   "literal_positive_float"  },
  {TokenType::literal_negative_float,   "literal_negative_float"  },

  {TokenType::invalid,                  "invalid"                 },

  {TokenType::key_void,                 "key_void"                },
  {TokenType::key_int,                  "key_int"                 },
  {TokenType::key_long,                 "key_long"                },
  {TokenType::key_double,               "key_double"              },
  {TokenType::key_char,                 "key_char"                },
  {TokenType::key_unsigned,             "key_unsigned"            },
  {TokenType::key_do,                   "key_do"                  },
  {TokenType::key_default,              "default"                 },

  {TokenType::bit_and,                  "bit_and"                 },
  {TokenType::bit_or,                   "bit_or"                  },
  {TokenType::bit_not,                  "bit_not"                 },
  {TokenType::log_and,                  "log_and"                 },
  {TokenType::log_or,                   "log_or"                  },
  {TokenType::log_not,                  "log_not"                 },
  {TokenType::plus,                     "plus"                    },
  {TokenType::minus,                    "minus"                   },
  {TokenType::times,                    "times"                   },
  {TokenType::div,                      "div"                     },
  {TokenType::mod,                      "mod"                     },
  {TokenType::arrow,                    "arrow"                   },
  {TokenType::equ,                      "equ"                     },
  {TokenType::not_equ,                  "not_equ"                 },
  {TokenType::great_equ,                "great_equ"               },
  {TokenType::less_equ,                 "less_equ"                },
  {TokenType::great,                    "great"                   },
  {TokenType::less,                     "less"                    },
  {TokenType::assign,                   "init_branch_type"        },
  {TokenType::plus_agn,                 "plus_agn"                },
  {TokenType::minus_agn,                "minus_agn"               },
  {TokenType::times_agn,                "times_agn"               },
  {TokenType::div_agn,                  "div_agn"                 },
  {TokenType::mod_agn,                  "mod_agn"                 },
  {TokenType::l_par,                    "l_par"                   },
  {TokenType::r_par,                    "r_par"                   },
  {TokenType::l_mid_par,                "l_mid_par"               },
  {TokenType::r_mid_par,                "r_mid_par"               },
  {TokenType::l_big_par,                "l_big_par"               },
  {TokenType::r_big_par,                "r_big_par"               },
  {TokenType::colon,                    "colon"                   },
  {TokenType::self_plus,                "self_plus"               },
  {TokenType::self_minus,               "self_minus"              },
  {TokenType::comma,                    "comma"                   },
  {TokenType::semicolon,                "semicolon"               },
  {TokenType::identify,                 "identify"                },
  {TokenType::key_if,                   "key_if"                  },
  {TokenType::key_else,                 "key_else"                },
  {TokenType::key_true,                 "key_true"                },
  {TokenType::key_false,                "key_false"               },
  {TokenType::key_while,                "key_while"               },
  {TokenType::key_for,                  "key_for"                 },
  {TokenType::key_null,                 "key_null"                },
  {TokenType::key_func,                 "key_func"                },
  {TokenType::key_return,               "key_return"              },
  {TokenType::key_break,                "key_break"               },
  {TokenType::key_continue,             "key_continue"            },
  {TokenType::key_struct,               "key_struct"              },
  {TokenType::key__Bool,                "key__Bool"               },
  {TokenType::key__Complex,             "key__Complex"            },
  {TokenType::key__Imaginary,           "key__Imaginary"          },
  {TokenType::key_auto,                 "key_auto"                },
  {TokenType::key_break,                "key_break"               },
  {TokenType::key_case,                 "key_case"                },
  {TokenType::key_enum,                 "key_enum"                },
  {TokenType::key_extern,               "key_extern"              },
  {TokenType::key_restrict,             "key_restrict"            },
  {TokenType::key_switch,               "key_switch"              },

  {TokenType::eof,                      "eof"                     },
};

}  // namespace toy_c::token