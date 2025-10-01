cmake_minimum_required(VERSION 3.13)

project(luna CXX)

set(FFTW_DIR ${CMAKE_SOURCE_DIR}/libs/fftw335dll64)
set(SNDF_DIR ${CMAKE_SOURCE_DIR}/libs/libsndfile122win64)

add_executable(luna main.cpp)

target_include_directories(luna PRIVATE
    ${FFTW_DIR}
    "${SNDF_DIR}/include"
)

target_link_libraries(luna PRIVATE
    "${FFTW_DIR}/libfftw3-3.lib"
    "${FFTW_DIR}/libfftw3f-3.lib"
    "${FFTW_DIR}/libfftw3l-3.lib"
    "${SNDF_DIR}/lib/sndfile.lib"
)

set(FFTW_DLLS
    libfftw3-3.dll
    libfftw3f-3.dll
    libfftw3l-3.dll
)

foreach(DLL_NAME IN ITEMS ${FFTW_DLLS})
    add_custom_command(
        TARGET luna
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FFTW_DIR}/${DLL_NAME}"
            "$<TARGET_FILE_DIR:luna>/${DLL_NAME}"
    )
endforeach()

add_custom_command(
    TARGET luna
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SNDF_DIR}/bin/sndfile.dll"
        "$<TARGET_FILE_DIR:luna>/sndfile.dll"
)