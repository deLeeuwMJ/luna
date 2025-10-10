add_compile_definitions(WITH_WINMM)

set(SOLOUD_SRC_DIR "${LIBS_DIR}/soloud20200207/src")
set(SOLOUD_INC_DIR "${LIBS_DIR}/soloud20200207/include")

set(SOLOUD_SOURCE
  ${SOLOUD_SRC_DIR}/audiosource/monotone/soloud_monotone.cpp
  ${SOLOUD_SRC_DIR}/audiosource/noise/soloud_noise.cpp
  ${SOLOUD_SRC_DIR}/audiosource/openmpt/soloud_openmpt.cpp
  ${SOLOUD_SRC_DIR}/audiosource/openmpt/soloud_openmpt_dll.c
  ${SOLOUD_SRC_DIR}/audiosource/sfxr/soloud_sfxr.cpp
  ${SOLOUD_SRC_DIR}/audiosource/speech/darray.cpp
  ${SOLOUD_SRC_DIR}/audiosource/speech/klatt.cpp
  ${SOLOUD_SRC_DIR}/audiosource/speech/resonator.cpp
  ${SOLOUD_SRC_DIR}/audiosource/speech/soloud_speech.cpp
  ${SOLOUD_SRC_DIR}/audiosource/speech/tts.cpp
  ${SOLOUD_SRC_DIR}/audiosource/tedsid/sid.cpp
  ${SOLOUD_SRC_DIR}/audiosource/tedsid/soloud_tedsid.cpp
  ${SOLOUD_SRC_DIR}/audiosource/tedsid/ted.cpp
  ${SOLOUD_SRC_DIR}/audiosource/vic/soloud_vic.cpp
  ${SOLOUD_SRC_DIR}/audiosource/vizsn/soloud_vizsn.cpp
  ${SOLOUD_SRC_DIR}/audiosource/wav/dr_impl.cpp
  ${SOLOUD_SRC_DIR}/audiosource/wav/soloud_wav.cpp
  ${SOLOUD_SRC_DIR}/audiosource/wav/soloud_wavstream.cpp
#   ${SOLOUD_SRC_DIR}/audiosource/wav/stb_vorbis.c
  ${SOLOUD_SRC_DIR}/backend/null/soloud_null.cpp
  ${SOLOUD_SRC_DIR}/backend/winmm/soloud_winmm.cpp
  ${SOLOUD_SRC_DIR}/backend/alsa/soloud_alsa.cpp
  ${SOLOUD_SRC_DIR}/core/soloud.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_audiosource.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_bus.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_3d.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_basicops.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_faderops.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_filterops.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_getters.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_setters.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_voicegroup.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_core_voiceops.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_fader.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_fft.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_fft_lut.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_file.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_filter.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_misc.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_queue.cpp
  ${SOLOUD_SRC_DIR}/core/soloud_thread.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_bassboostfilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_biquadresonantfilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_dcremovalfilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_echofilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_fftfilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_flangerfilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_freeverbfilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_lofifilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_robotizefilter.cpp
  ${SOLOUD_SRC_DIR}/filter/soloud_waveshaperfilter.cpp
)

set(SDL_SDK_DIR "${LIBS_DIR}/SDL33224")
set(SDL_BIN_DIR "${LIBS_DIR}/SDL33224/bin")

add_subdirectory(${SDL_SDK_DIR} ${SDL_BIN_DIR} EXCLUDE_FROM_ALL)

include_directories(
    ${SOLOUD_INC_DIR} 
)

add_library(SOLOUD STATIC 
	${SOLOUD_SOURCE}
	${SOLOUD_INCLUDE}
)

add_executable(rhythm_game 
    main.cpp
    core/sound_engine.cpp
    core/render_engine.cpp
    component/map_loader.cpp
)

target_link_libraries( rhythm_game PRIVATE
    SOLOUD
    winmm
    nlohmann_json::nlohmann_json
    SDL3::SDL3
)

set_target_properties(rhythm_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
