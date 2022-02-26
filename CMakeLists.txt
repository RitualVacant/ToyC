cmake_minimum_required(VERSION 3.0.0)

#set(VCPKG_ROOT "/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
#set(DCMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})

project(MyScirpt VERSION 0.1.0)

include("/home/lzj/software/vcpkg/scripts/buildsystems/vcpkg.cmake")
enable_testing()

add_executable(MyScirpt main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)