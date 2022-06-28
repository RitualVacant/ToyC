cmake_minimum_required(VERSION 3.13.4)


set(CMAKE_CXX_COMPILER "/usr/local/bin/clang")
set(CMAKE_C_COMPILER "/usr/local/bin/clang")
#set(VCPKG_ROOT "/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
#set(DCMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

project(ToyC VERSION 0.1.0)
enable_testing()
add_executable(ToyC main.cpp)
add_definitions("-O1 -Wall -g -w")
add_compile_options("-DLLVM_ENABLE_DUMP=ON")

set(CMAKE_BUILD_TYPE "Debug")
set(CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#
#LLVM
#
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
# Now build our tools
#add_executable(simple-tool tool.cpp)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)
# Link against LLVM libraries
target_link_libraries(ToyC ${llvm_libs})




#include_directories(/usr/include/llvm-12)
#link_directories(/usr/include/llvm-12)

#include_directories(/usr/include/llvm-c-12)
#link_directories(/usr/include/llvm-c-12)


#set(CMAKE_CXX_FLAGS_RELEASE "-O1 -Wall -g -lpython3.8")

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
#target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#find_package (Python COMPONENTS Interpreter Development)
#message(STATUS "Python_VERSION: ${Python_INCLUDE_DIRS}")
#include_directories(
#    ${PROJECT_SOURCE_DIR}/include
#    ${Python_INCLUDE_DIRS}
#    )
#target_link_libraries(${PROJECT_NAME} ${Python_LIBRARIES})