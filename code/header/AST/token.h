#ifndef TOKEN_H
#define TOKEN_H
enum class token {
    invalid = 0,


    jmp,     //*jmp 用于while if 的
    lable,   //*

    //声明
    dec_int,    //*
    dec_double, //*
    dec_char,   //*
    dec_bool,   //*

    dec_int_arrary,     //*
    dec_double_arrary,  //*
    dec_char_arrary,    //*
    dec_bool_arrary,    //*

    dec_func,           //*
    call_func,          //*
    call_func_end,      //*
    func_return,        //*


    arg_r_int,            //*函数参数调用立即数
    arg_r_double,         //*函数参数调用立即数
    arg_r_char,           //*函数参数调用立即数
    arg_r_bool,           //*函数参数调用立即数

    arg_int_class,      //*函数参数调用
    arg_double_class,   //*函数参数调用
    arg_char_class,     //*函数参数调用
    arg_bool_class,     //*函数参数调用

    class_int,     //int
    class_double,  //double
    class_char,    //char
    class_bool,    //bool

    class_int_array,     //int array
    class_double_array,  //double array
    class_char_array,    //char array
    class_bool_array,

    class_int_array_unit,   // + - * /
    class_double_array_unit,
    class_char_array_unit,
    class_bool_array_unit,

    arrary_unit, //TODO 数组运算到底要不要分出类型， 这里暂时不分用这个代替

    key_void,
    key_int,
    key_double,
    key_string,
    key_char,
    key_bool,

    r_int,
    r_double,
    r_char,
    r_bool,


    bit_and, //&
    bit_or,  //|
    bit_not, //~

    log_and, //&&
    log_or,  //||
    log_not, //~

    plus,   //+
    minus,  //-
    times,  //*
    div,    ///
    mod,    //%

    ver,        //->
    equ,        //==
    not_equ,    //!=
    great_equ,  //>=
    less_equ,   //<=
    great,      //>
    less,       //<


    assign,     //=
    plus_agn,   //+=
    minus_agn,  //-=
    times_agn,  //*=
    div_agn,    ///=
    mod_agn,    //%=

    l_par,       //(
    r_par,       //)
    l_mid_par,   //[
    r_mid_par,   //]
    l_big_par,   //{
    r_big_par,   //}

    comma,   //,
    comment, //#
    end,     //;

    indentif,  //标识符
    key_if,         //if
    key_else,       //else
    key_true,       //true
    key_false,      //false
    key_while,      //while
    key_for,        //for
    key_null,       //null
    key_func,       //func
    key_return,     //return
    key_break,      //break
    key_continue,   //continue
};
#endif