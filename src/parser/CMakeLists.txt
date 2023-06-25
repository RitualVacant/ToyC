file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(parser STATIC ${srcs} src/ParserImpl.cpp)
target_include_directories(parser PUBLIC include)
target_link_libraries(parser PUBLIC scan ast)
