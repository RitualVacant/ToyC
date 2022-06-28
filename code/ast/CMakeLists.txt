file(GLOB_RECURSE src CONGIGURE_DEPENDS *.cpp *.h)
add_library(ast STATIC ${src})
set_target_properties(ast PROPERTIES LINKER_LANGUAGE CXX)