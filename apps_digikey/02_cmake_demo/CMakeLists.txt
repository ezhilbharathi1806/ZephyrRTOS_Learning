cmake_minimum_required(VERSION 3.20)

project(hello_world LANGUAGES C)

add_library(mylib STATIC
    src/mylib.c
)

target_include_directories(mylib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(hello_world
    main.c
)

target_link_libraries(hello_world PRIVATE
    mylib
)