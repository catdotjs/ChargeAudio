cmake_minimum_required(VERSION 3.10)

project(ChargeAudio VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_MODULE_PATH "modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif()
add_subdirectory(lib/miniaudio)

find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWRESAMPLE REQUIRED libswresample)

add_library(ChargeAudio SHARED "src/ChargeAudio.hpp" "src/Engine.cpp"
                               "src/Sound.cpp" "lib/miniaudio/miniaudio.c")

target_link_libraries(
  ChargeAudio
  PRIVATE Corrade::Main Magnum::Magnum ${AVFORMAT_LIBRARIES}
          ${AVCODEC_LIBRARIES} ${AVUTIL_LIBRARIES} ${SWRESAMPLE_LIBRARIES})

target_include_directories(
  ChargeAudio
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
         $<INSTALL_INTERFACE:include>)

# Library
install(
  TARGETS ChargeAudio
  EXPORT ChargeAudioTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)

# include
install(FILES src/ChargeAudio.hpp DESTINATION include/Charge)
install(FILES lib/miniaudio/miniaudio.h DESTINATION include/Charge/miniaudio/)

install(
  EXPORT ChargeAudioTargets
  FILE ChargeAudioTargets.cmake
  NAMESPACE ChargeAudio::
  DESTINATION lib/cmake/ChargeAudio)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ChargeAudioConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/ChargeAudioConfig.cmake"
  INSTALL_DESTINATION lib/cmake/ChargeAudio)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ChargeAudioConfig.cmake"
        DESTINATION lib/cmake/ChargeAudio)
