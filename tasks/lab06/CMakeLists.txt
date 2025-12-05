cmake_minimum_required(VERSION 3.14)
project(BalagurFate3Editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Основная программа
add_executable(editor
    src/main.cpp
    src/NPC.cpp
    src/Dragon.cpp
    src/Bull.cpp
    src/Frog.cpp
    src/NPCFactory.cpp
    src/BattleVisitor.cpp
    src/Observer.cpp
    src/Editor.cpp
)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/test_main.cpp
    src/NPC.cpp
    src/Dragon.cpp
    src/Bull.cpp
    src/Frog.cpp
    src/NPCFactory.cpp
    src/BattleVisitor.cpp
    src/Observer.cpp
    src/Editor.cpp
)

target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
