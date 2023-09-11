cmake_minimum_required(VERSION 3.22)
project(Ultimate3TAI)

# Main project build setup
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/code/source/*.cpp)
add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp ${SRC_FILES})
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code/headers)