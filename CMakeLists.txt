cmake_minimum_required(VERSION 3.15)

project(ANTARA VERSION 1.0.0)

add_library(antaralib)
add_library(ANTARA::antaralib ALIAS antaralib)

option(BUILD_TEST "Build Test Files" OFF)

include(GNUInstallDirs)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/antara-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ANTARA")

if(MSVC)
  target_compile_options(antaralib PRIVATE /W4 /WX)
else()
  target_compile_options(antaralib PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(core)


if(BUILD_TESTS)
  add_subdirectory(tests)
endif()