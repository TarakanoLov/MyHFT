cmake_minimum_required(VERSION 3.0)

project(MyHFT)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -Werror -Wpedantic")
set(CMAKE_VERBOSE_MAKEFILE on)

#add_library(libcommon STATIC ${SOURCES})

#list(APPEND LIBS libcommon)
#list(APPEND LIBS pthread)

include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES
    src/*/*.h
    src/*/*.cpp
    src/main.cpp
)

add_executable(monkey.exe ${SOURCES})
target_link_libraries(monkey.exe PUBLIC ${LIBS})

# tests

enable_testing()
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES
    src/*/*.h
    tests/*/*.h
    tests/*/*.cpp
)

add_executable(tests.exe ${TEST_SOURCES})

target_link_libraries(
    tests.exe
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests.exe)