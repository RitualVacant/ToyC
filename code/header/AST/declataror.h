//TODO

#ifndef DECLARAROR_H
#define DECLARAROR_H

enum class declarator_type : unsigned char {
    type_empty,

    type_void,
    type_int,
    type_short_int,
    type_char,
    type_float,
    type_double,
    type_long_int,
    type_long_long_int,
};

enum class declarator_store : unsigned char {
    store_empty,

    store_extern,
    store_static,
    store_typedef
};

enum class declarator_limit : unsigned char {
    limit_empty,
    limit_Alignas
};

enum class declarator_sign : bool {
    sign_signed,
    sign_unsigned
};

struct declaration_declarator {
    declarator_type type = declarator_type::type_empty;
    declarator_store store = declarator_store::store_empty;
    declarator_limit limit = declarator_limit::limit_empty;
    declarator_sign sign = declarator_sign::sign_signed;
};

#endif