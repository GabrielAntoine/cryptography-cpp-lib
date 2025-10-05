add_subdirectory(algorithms/des)

add_library(cryptography_cpp_lib INTERFACE)

target_link_libraries(cryptography_cpp_lib INTERFACE
    des
)
