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
        int bp = 0;
        int sp = 0;
        std::vector<std::tuple<token, std::string>> sign_stack;
        /*
        按道理应该这样写的,c++标准库居然不支持塞这种类，不能生成哈希值
        只能塞string了，坑死人
        std::unordered_set<std::tuple<token, std::string>> global_sign;
        */

        std::unordered_set<std::string> global_sign;

    public:
        void push_token(std::tuple<token, std::string>);
        void push_global_sign(std::tuple<token, std::string>);

        void push_func(std::tuple<token, std::string> x);
        void pop_func(std::tuple<token, std::string> x);
};
