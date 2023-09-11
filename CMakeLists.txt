cmake_minimum_required(VERSION 3.22)
project(Ultimate3TAI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main project build setup
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/code/source/*.cpp)
add_executable(main ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp ${SRC_FILES})
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code/headers)


# For Testing Project
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TEST_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/code/tests/*.cpp)
add_executable(
    tests
    ${TEST_INCLUDE_FILES}
    ${SRC_FILES}
)
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code/headers)

target_link_libraries( 
    tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)