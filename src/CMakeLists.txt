cmake_minimum_required(VERSION 3.27)
project(hash-table C)
set(CMAKE_CXX_STANDARD 11)
add_executable(main
        main.c
        hash_table.c
)