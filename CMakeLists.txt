cmake_minimum_required(VERSION 3.0.0)

#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
#set(CMAKE_C_COMPILER "/usr/bin/clang-12")


#set(VCPKG_ROOT "/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
#set(DCMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
project(ToyC VERSION 0.1.0)

enable_testing()

add_executable(ToyC main.cpp)
add_definitions("-O1 -Wall -g -I /usr/include/python3.8 -l python3.8")
include_directories(/usr/include/python3.8)
link_directories(/usr/lib/python3.8)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS_RELEASE "-O1 -Wall -g -lpython3.8")


set(CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#find_package (Python COMPONENTS Interpreter Development)
#message(STATUS "Python_VERSION: ${Python_INCLUDE_DIRS}")
#include_directories(
#    ${PROJECT_SOURCE_DIR}/include
#    ${Python_INCLUDE_DIRS}
#    )
#target_link_libraries(${PROJECT_NAME} ${Python_LIBRARIES})