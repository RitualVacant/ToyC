#ifndef TOKEN_H
#define TOKEN_H
enum class token : unsigned int {
  invalid = 0,

  jmp,    //*jmp 用于while if 的
  label,  //*

  //声明
  dec_int,     //*
  dec_double,  //*
  dec_char,    //*
  dec_bool,    //*

  dec_int_arrary,     //*
  dec_double_arrary,  //*
  dec_char_arrary,    //*
  dec_bool_arrary,    //*

  dec_func,  //*
  def_func,
  call_func,      //*
  call_func_end,  //*
  func_return,    //*

  arg_r_int,     //*函数参数调用立即数
  arg_r_double,  //*函数参数调用立即数
  arg_r_char,    //*函数参数调用立即数
  arg_r_bool,    //*函数参数调用立即数

  arg_int_class,     //*函数参数调用
  arg_double_class,  //*函数参数调用
  arg_char_class,    //*函数参数调用
  arg_bool_class,    //*函数参数调用

  class_int,     // int
  class_double,  // double
  class_char,    // char
  class_bool,    // bool

  class_int_array,     // int array
  class_double_array,  // double array
  class_char_array,    // char array
  class_bool_array,

  class_int_array_unit,  // + - * /
  class_double_array_unit,
  class_char_array_unit,
  class_bool_array_unit,

  arrary_unit,  // TODO 数组运算到底要不要分出类型， 这里暂时不分用这个代替

  r_int,
  r_double,
  r_char,
  r_bool,

  key_quotation,  //:

  bit_and,  //&
  bit_or,   //|
  bit_not,  //~
  bit_xor,  //^

  log_and,  //&&
  log_or,   //||
  log_not,  //!

  plus,    //+
  minus,   //-
  times,   //*
  div,     ///
  mod,     //%
  period,  //.

  self_plus,   //++
  self_minus,  //--

  ver,        //->
  equ,        //==
  not_equ,    //!=
  great_equ,  //>=
  less_equ,   //<=
  great,      //>
  less,       //<
  l_shift,    //<<
  r_shift,    //>>

  assign,       //=
  plus_agn,     //+=
  minus_agn,    //-=
  times_agn,    //*=
  div_agn,      ///=
  mod_agn,      //%=
  r_shift_agn,  //>>=
  l_shift_agn,  //<<=
  bit_and_agn,  //&=
  bit_or_agn,   //|=
  bit_xor_agn,  //^=

  question_mark,  //?

  l_par,      //(
  r_par,      //)
  l_mid_par,  //[
  r_mid_par,  //]
  l_big_par,  //{
  r_big_par,  //}

  comma,    //,
  comment,  //#
  end,      //;

  identify,  //标识符
  key_auto,
  key_const,
  key_enum,
  key_inline,      // inline
  key_register,    // register
  key_restrict,    // restrict
  key_signed,      // signed
  key_short,       // short
  key_sizeof,      // sizeof
  key_static,      // static
  key_typedef,     // typedef
  key_union,       // union
  key_void,        // void
  key_volatile,    // volatile
  key__Bool,       // Bool
  key__Complex,    // Complex
  key__Imaginary,  // Imaginary

  key_extern,  // extern
  // key type
  key_int,
  key_long,
  key_double,
  key_float,
  key_unsigned,
  key_char,
  key_struct,

  key_if,        // if
  key_else,      // else
  key_true,      // true
  key_false,     // false
  key_while,     // while
  key_goto,      // goto
  key_for,       // for
  key_null,      // null
  key_func,      // func
  key_return,    // return
  key_break,     // break
  key_continue,  // continue
  key_switch,    // swtich
  key_do,        // do
  key_case,      // case
  key_default    // default
};
#endif