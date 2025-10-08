set(FFTW_DIR ${CMAKE_SOURCE_DIR}/_libs/fftw335dll64)
set(SNDF_DIR ${CMAKE_SOURCE_DIR}/_libs/libsndfile122win64)

add_executable(map_generator main.cpp)

target_include_directories(map_generator PRIVATE
    ${FFTW_DIR}
    "${SNDF_DIR}/include"
)

target_link_libraries(map_generator PRIVATE
    "${FFTW_DIR}/libfftw3-3.lib"
    "${FFTW_DIR}/libfftw3f-3.lib"
    "${FFTW_DIR}/libfftw3l-3.lib"
    "${SNDF_DIR}/lib/sndfile.lib"
    nlohmann_json::nlohmann_json
)

set(FFTW_DLLS
    libfftw3-3.dll
    libfftw3f-3.dll
    libfftw3l-3.dll
)

foreach(DLL_NAME IN ITEMS ${FFTW_DLLS})
    add_custom_command(
        TARGET map_generator
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FFTW_DIR}/${DLL_NAME}"
            "$<TARGET_FILE_DIR:map_generator>/${DLL_NAME}"
    )
endforeach()

add_custom_command(
    TARGET map_generator
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SNDF_DIR}/bin/sndfile.dll"
        "$<TARGET_FILE_DIR:map_generator>/sndfile.dll"
)