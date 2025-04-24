get_filename_component(ANTARA_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(NOT TARGET ANTARA::antaralib)
    include("${ANTARA_CMAKE_DIR}/antara-targets.cmake")    
endif()

