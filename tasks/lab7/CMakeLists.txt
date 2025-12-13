cmake_minimum_required(VERSION 3.14)
project(lab07)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основные исходники
set(SOURCES
    src/NPC.cpp
    src/NPCTypes.cpp
    src/NPCFactory.cpp
    src/Observer.cpp
    src/BattleVisitor.cpp
    src/GameWorld.cpp
)

# Заголовочные файлы
include_directories(include)

# Исполняемый файл демо
add_executable(lab07_demo main.cpp ${SOURCES})
target_link_libraries(lab07_demo pthread)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты
add_executable(lab07_tests tests/test_lab07.cpp ${SOURCES})
target_link_libraries(lab07_tests gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(lab07_tests)
