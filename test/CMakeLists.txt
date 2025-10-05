add_executable(test_app 
    test-des.cpp
)

target_link_libraries(test_app PRIVATE cryptography_cpp_lib)