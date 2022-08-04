cmake_minimum_required(VERSION 3.13.4)


set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++")
set(CMAKE_C_COMPILER "/usr/local/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")
#set(CMAKE_C_COMPILER "/usr/bin/g++-11")
#set(VCPKG_ROOT "/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
#set(DCMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

project(ToyC VERSION 0.1.0 LANGUAGES CXX)
enable_testing()
add_executable(ToyC main.cpp)
add_definitions("-std=c++20")

include(ProcessorCount)
ProcessorCount(N)
message("number of processors:"  ${N})
set(CMAKE_BUILD_PAR)

#set(CMAKE_CODEBLOCKS_MAKE_ARGUMENTS -j12)

message("cmake make program" ${CMAKE_MAKE_PROGRAM})
#set(CMAKE_CXX_COMPILER_ID )
set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
set(CMAKE_BUILD_PARALLEL_LEVEL 12)
set(CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR "Ninja")

#file(GLOB_RECURSE source CONFIGURE_DEPENDS *.cpp *.h)
#target_link_libraries(ToyC PUBLIC ./code)


#add_subdirectory(code)
#target_include_directories(ToyC PUBLIC ./code)

#target_sources(ToyC PUBLIC ${source})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/ast)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/class/implement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/class/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code/error)

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
#set(CMAKE_CXX_FLAGS_RELEASE "-O1 -Wall -g -lpython3.8")

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt ${llvm_libs})
#target_link_libraries(${PROJECT_NAME} PUBLIC ${llvm_libs})

#target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt ${llvm_libs} ${source})
#target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#find_package (Python COMPONENTS Interpreter Development)
#message(STATUS "Python_VERSION: ${Python_INCLUDE_DIRS}")
#include_directories(
#    ${PROJECT_SOURCE_DIR}/include
#    ${Python_INCLUDE_DIRS}
#    )
#target_link_libraries(${PROJECT_NAME} ${Python_LIBRARIES})