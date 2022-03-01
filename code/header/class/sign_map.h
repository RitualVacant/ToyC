#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef TUPLR
#define TUPLR
#include <tuple>
#endif

#ifndef UNORDER_SET
#define UNORDER_SET
#include <unordered_set>
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "./../AST/token.h"
#endif

#ifndef STRING
#define STRING
#include <string>
#endif
//
class sign_map {
    private:
        //
        std::vector<std::tuple<token, std::string>> sign_stack;
        std::unordered_set<std::tuple<token, std::string>> global_sign;

        //

    public:
        void push_token(std::tuple<token, std::string>);
        void push_global_sign(std::tuple<token, std::string>);

        void push_func();
        void pop_func();
};