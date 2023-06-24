file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(scan STATIC ${srcs})
target_include_directories(scan PUBLIC include)
target_link_libraries(scan PUBLIC fmt::fmt token controller)