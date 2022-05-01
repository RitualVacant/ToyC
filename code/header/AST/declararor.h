//TODO

#ifndef DECLARAROR_H
#define DECLARAROR_H

enum class declatator_type : unsigned char {
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

enum class declatator_store : unsigned char {
    store_empty,

    store_extern,
    store_static,
    store_typedef
};

enum class declatator_limit : unsigned char {
    limit_empty,
    limit_Alignas
};

enum class declatator_sign : bool {
    sign_signed,
    sign_unsigned
};

struct declataror {
    declatator_sign sign;
    declatator_type type;
    declatator_store store;
    declatator_limit limit;
    declataror() : type{declatator_type::type_empty},
                   store{declatator_store::store_empty},
                   limit{declatator_limit::limit_empty},
                   sign{declatator_sign::sign_signed} {};
};

#endif