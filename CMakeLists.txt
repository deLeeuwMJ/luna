cmake_minimum_required(VERSION 3.13)

project(luna CXX C)

set(GLOBAL_EXECUTABLE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/bin")
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/_libs)

add_subdirectory(map_generator)
add_subdirectory(rhythm_game)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
