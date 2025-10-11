add_library(cryptography_cpp_lib STATIC
    DES/DES.cpp
    DESede/DESede.cpp
    DES/DESSecretKey.cpp
    DESede/DESedeSecretKey.cpp
    
    utils/bytes_stream.cpp
    utils/bytes.cpp
)

target_include_directories(cryptography_cpp_lib 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/include/detail # In public for development purposes
    PUBLIC ${PROJECT_SOURCE_DIR}/include/template
    PUBLIC ${PROJECT_SOURCE_DIR}/include/concepts
)