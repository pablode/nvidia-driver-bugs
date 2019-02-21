cmake_minimum_required(VERSION 3.11)

project(nvidia-driver-bugs C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(extern)
add_subdirectory(src)
