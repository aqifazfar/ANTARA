add_library(ANTARA::antaralib IMPORTED)

set_target_properties(ANTARA::antaralib
                      PROPERTIES
                      INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include")