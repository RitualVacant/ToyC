//
// Created by lzj on 2023.6.23.
//

#ifndef TOYC_TYPE_SPECIFIER_AND_QUALIFIER_H
#define TOYC_TYPE_SPECIFIER_AND_QUALIFIER_H


#include <map>
namespace toy_c::ast
{

enum class TypeSpecifier
{
  _empty_ = 0,

  _void_,
  _char_,
  _short_,
  _int_,
  _long_,
  _long_long_,

  _float_,
  _double_,

  __Bool_,
  __Complex_,

  _struct_,
  _union_,
  _enum_,

};

enum class StoreClassSpecifier
{
  _empty_,
  _typedef_,
  _extern_,
  _static_,
  _auto_,
  _register_,
};

enum class TypeQualifier
{
  _empty_,
  _const_,
  _restrict_,
  _volatile_,
};

enum class FunctionSpecifier
{
  _empty_,
  _inline_,
};

inline std::map<TypeQualifier, std::string_view> typeQualifierMap = {
  {TypeQualifier::_empty_,    "empty"   },
  {TypeQualifier::_const_,    "const"   },
  {TypeQualifier::_restrict_, "restrict"},
  {TypeQualifier::_volatile_, "volatile"},
};

inline std::map<TypeSpecifier, std::string_view> typeSpecifierMap = {
  {TypeSpecifier::_empty_,     "empty"    },
  {TypeSpecifier::_void_,      "void"     },
  {TypeSpecifier::_char_,      "char"     },
  {TypeSpecifier::_short_,     "short"    },
  {TypeSpecifier::_int_,       "int"      },
  {TypeSpecifier::_long_,      "long"     },
  {TypeSpecifier::_long_long_, "long long"},
  {TypeSpecifier::_float_,     "float"    },
  {TypeSpecifier::_double_,    "double"   },
  {TypeSpecifier::__Bool_,     "_Bool"    },
  {TypeSpecifier::__Complex_,  "_Complex" },
  {TypeSpecifier::_struct_,    "struct"   },
  {TypeSpecifier::_union_,     "union"    },
  {TypeSpecifier::_enum_,      "enum"     },
};

inline std::map<FunctionSpecifier, std::string_view> functionSpecifierMap = {
  {FunctionSpecifier::_empty_,  "empty" },
  {FunctionSpecifier::_inline_, "inline"},
};

inline std::map<StoreClassSpecifier, std::string_view> storeClassSpecifierMap = {
  {StoreClassSpecifier::_empty_,    "empty"   },
  {StoreClassSpecifier::_typedef_,  "typedef" },
  {StoreClassSpecifier::_extern_,   "extern"  },
  {StoreClassSpecifier::_static_,   "static"  },
  {StoreClassSpecifier::_auto_,     "auto"    },
  {StoreClassSpecifier::_register_, "register"},
};

}  // namespace toy_c::ast


#endif  // TOYC_TYPE_SPECIFIER_AND_QUALIFIER_H
