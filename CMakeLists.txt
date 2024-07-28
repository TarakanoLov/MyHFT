cmake_minimum_required(VERSION 3.0)

project(MyHFT)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++2a")
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(CURL REQUIRED)

# Download RapidJSON
include(ExternalProject)
ExternalProject_Add(
    rapidjson
    PREFIX "vendor/rapidjson"
    GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
    GIT_TAG f54b0e47a08782a6131cc3d60f94d038fa6e0a51
    TIMEOUT 10
    CMAKE_ARGS
        -DRAPIDJSON_BUILD_TESTS=OFF
        -DRAPIDJSON_BUILD_DOC=OFF
        -DRAPIDJSON_BUILD_EXAMPLES=OFF
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)
ExternalProject_Get_Property(rapidjson source_dir)
set(RAPIDJSON_INCLUDE_DIR ${source_dir}/include)

include_directories(${CURL_INCLUDE_DIR})
include_directories(${RAPIDJSON_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES
    src/*/*.h
    src/*/*.cpp
    src/main.cpp
)

add_executable(hft.exe ${SOURCES})
target_link_libraries(hft.exe PUBLIC ${CURL_LIBRARIES})




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