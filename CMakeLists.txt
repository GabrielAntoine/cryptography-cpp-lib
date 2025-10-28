cmake_minimum_required(VERSION 3.30.4)
project(cryptography_cpp_lib)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

add_subdirectory(src)
add_subdirectory(test)
