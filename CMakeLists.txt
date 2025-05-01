cmake_minimum_required(VERSION 3.15)

project(ANTARA VERSION 1.0.0 LANGUAGES CXX)

add_library(antaralib)
add_library(ANTARA::antaralib ALIAS antaralib)

option(ANTARA_BUILD_TESTS "Build ANTARA Test Files" OFF)

set_target_properties(antaralib PROPERTIES LINKER_LANGUAGE CXX)

include(GNUInstallDirs)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/antara-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ANTARA")
    
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)
endif()

if(MSVC)
  target_compile_options(antaralib PRIVATE /W4 /WX)
else()
  target_compile_options(antaralib PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-exceptions)
endif()

add_subdirectory(core)

if(ANTARA_BUILD_TESTS)
  message("ANTARA Tests Files will be build")
  add_subdirectory(tests)
endif()