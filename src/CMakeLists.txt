add_library(cryptography_cpp_lib STATIC
    DES/DES.cpp
    DESede/DESede.cpp
    DES/DESSecretKey.cpp
    DESede/DESedeSecretKey.cpp
    
    utils/bytes_stream.cpp
    utils/galois.cpp
)

target_include_directories(cryptography_cpp_lib 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/include/concepts

    PUBLIC ${PROJECT_SOURCE_DIR}/template
    PUBLIC ${PROJECT_SOURCE_DIR}/template/encryption
    PUBLIC ${PROJECT_SOURCE_DIR}/template/modes
    PUBLIC ${PROJECT_SOURCE_DIR}/template/paddings
    PUBLIC ${PROJECT_SOURCE_DIR}/template/utils
)