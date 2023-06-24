cmake_minimum_required(VERSION 3.20.0)

project(ToyC VERSION 0.1.0 LANGUAGES C CXX)

#
# platform check
#
if (MSVC)
    message(STATUS "windows msvc x64")
    set(CMAKE_GENERATOR_PLATFORM x64)
elseif (LINUX)
    message(STATUS "linux")
elseif (IOS)
    message(STATUS "ios")
endif ()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ToyC ${CMAKE_CURRENT_SOURCE_DIR} src/main.cpp)

message("")
message("")

# vcpkg
# set(VCPKG_ROOT "C:/Users/lzj/Dev/vcpkg")
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

message(STATUS "cmake make program" ${CMAKE_MAKE_PROGRAM})

#
# LLVM packages
#
message("LLVM")
set(LLVM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/llvm/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
# add_subdirectory(./lib/llvm-project/llvm)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS})
message(STATUS "LLVM include dirs ${LLVM_INCLUDE_DIRS}")
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
# Now build our tools
#add_executable(simple-tool tool.cpp)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)
# Link against LLVM libraries


#
# gflags
#
message("gflags")
set(gflags_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/gflags/)
find_package(gflags REQUIRED)
message(STATUS "Using gflags-config.cmake in ${gflags_DIR}")

#
# fmt packages
#
message("fmtlib")
set(fmt_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt)
find_package(fmt CONFIG REQUIRED)
message(STATUS "Using fmt-config.cmake in ${fmt_DIR}")

#
# nlohmann_json
#
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/json)


include_directories(src/macro/include)

add_subdirectory(src/controller)
add_subdirectory(src/scan)
add_subdirectory(src/token)
add_subdirectory(src/ast)
add_subdirectory(src/parser)
add_subdirectory(src/spec)

# link
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        fmt::fmt
        gflags
        nlohmann_json::nlohmann_json
        ${llvm_libs}
        controller
        scan
        token
        ast
        parser
        spec
)

message("DONE")

file(GLOB_RECURSE c_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/c_files/*.c)


message("ADD TEST")
message("")

set(output_file_extension_name_list "txt" "puml")
set(mode_list "scanner" "ast")

foreach (idx RANGE 1)
    foreach (c_file_path ${c_files})
        list(GET output_file_extension_name_list ${idx} extension_name)
        list(GET mode_list ${idx} mode)
        get_filename_component(c_file_name ${c_file_path} NAME_WE)
        set(test_name ${mode}_${c_file_name}_TEST)
        message(STATUS "add test ${test_name}")
        add_test(NAME ${test_name}
                COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ToyC -input ${c_file_path} -output ${CMAKE_CURRENT_SOURCE_DIR}/test/${mode}/${c_file_name}.${extension_name} -mode ${mode})
        set_tests_properties(${test_name} PROPERTIES TIMEOUT 3)
    endforeach ()
endforeach ()


enable_testing()

message("ADD TEST DONE ")

message("-- !OVER --")
