file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(controller STATIC ${srcs})
target_include_directories(controller PUBLIC include)
target_link_libraries(controller PUBLIC gflags fmt::fmt scan parser)