set(SOLOUD_DIR ${CMAKE_SOURCE_DIR}/_libs/soloud-RELEASE_20200207)

add_executable(rhythm_game main.cpp)

target_include_directories(rhythm_game PRIVATE
    "${SOLOUD_DIR}/include"
)

set_target_properties(rhythm_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
