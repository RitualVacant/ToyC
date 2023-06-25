file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(ast STATIC ${srcs})
target_include_directories(ast PUBLIC include)
target_link_libraries(ast
        PUBLIC
        token
        nlohmann_json::nlohmann_json
        fmt::fmt
        )
