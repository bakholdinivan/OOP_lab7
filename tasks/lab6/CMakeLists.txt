cmake_minimum_required(VERSION 3.10)
project(Lab06_Design_Patterns VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive- /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(lab06_lib 
    src/arena.cpp
    src/dragon.cpp
    src/bull.cpp
    src/frog.cpp
    src/npc.cpp
    src/factory.cpp
    src/combat_visitor.cpp
)

target_include_directories(lab06_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(dungeon_editor main.cpp)
target_link_libraries(dungeon_editor lab06_lib)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(lab06_tests 
    tests/test.cpp
)
target_link_libraries(lab06_tests 
    lab06_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lab06_tests)