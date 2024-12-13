#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ANTARA::antaralib" for configuration ""
set_property(TARGET ANTARA::antaralib APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ANTARA::antaralib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libantaralib.a"
  )

list(APPEND _cmake_import_check_targets ANTARA::antaralib )
list(APPEND _cmake_import_check_files_for_ANTARA::antaralib "${_IMPORT_PREFIX}/lib/libantaralib.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
