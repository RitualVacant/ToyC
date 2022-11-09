cmake_minimum_required(VERSION 3.13.4)

#set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++")
#set(CMAKE_C_COMPILER "/usr/local/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(ToyC VERSION 0.1.0 LANGUAGES CXX)
enable_testing()
add_executable(ToyC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_definitions("-std=c++20")

# set(VCPKG_ROOT "/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

#include(ProcessorCount)
#ProcessorCount(N)
#message("number of processors:"  ${N})
#set(CMAKE_BUILD_PAR)
#set(CMAKE_BUILD_PARALLEL_LEVEL 12)
#set(CMAKE_CODEBLOCKS_MAKE_ARGUMENTS -j12)

message("cmake make program" ${CMAKE_MAKE_PROGRAM})
#set(CMAKE_CXX_COMPILER_ID )
#set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
set(CMAKE_BUILD_TYPE Debug)
set(CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR "Ninja")

# header files h
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/token)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ast)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/error)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/var)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/spt)

# source files cpp
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/token tokenlib)
add_library(token STATIC ${tokenlib})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/implement implib)
add_library(implement STATIC ${implib})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/ast astlib)
add_library(ast STATIC ${astlib})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/var varlib)
add_library(var STATIC ${varlib})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/spt sptlib)
add_library(spt STATIC ${sptlib})


#
# LLVM packages
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

# fmt
find_package(fmt CONFIG REQUIRED)

# gflags
find_package(gflags REQUIRED)
message(STATUS "Using gflags-config.cmake in ${gflags_DIR}")

# link
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    fmt::fmt
    gflags
    ${llvm_libs}
    implement
    ast
    var
    spt
    token
    # profiler
    # unwind
)

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
