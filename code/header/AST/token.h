#ifndef TOKEN_H
#define TOKEN_H
enum class token {
    invalid = 0,

    class_int,     //int
    class_double,  //double
    class_string,  //string
    class_char,    //char
    class_bool,    //bool

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

    equ,        //==
    not_equ,       //!=
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