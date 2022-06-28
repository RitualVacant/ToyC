file(GLOB_RECURSE src CONGIGURE_DEPENDS *.cpp *.h)
add_library(error STATIC ${src})