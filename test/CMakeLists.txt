add_executable(test 
    test.cpp
)

add_executable(concept concepts.cpp)

target_link_libraries(test PRIVATE cryptography_cpp_lib)