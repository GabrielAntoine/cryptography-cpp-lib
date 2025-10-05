add_library(cryptography_cpp_lib STATIC
    DES/DES.cpp
    DESede/DESede.cpp
    DES/DESSecretKey.cpp
    DESede/DESedeSecretKey.cpp
)

target_include_directories(cryptography_cpp_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)