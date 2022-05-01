cmake_minimum_required(VERSION 3.0.0)

#set(VCPKG_ROOT "/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
#set(DCMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

project(ToyC VERSION 0.1.0)

enable_testing()

add_executable(ToyC main.cpp)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

set(CXX_STANDARD 20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)