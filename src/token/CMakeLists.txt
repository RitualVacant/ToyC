file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(token STATIC ${srcs})
target_include_directories(token PUBLIC include)