add_library(des STATIC
    des.cpp
    DesSecretKey.cpp
)

target_include_directories(des PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})