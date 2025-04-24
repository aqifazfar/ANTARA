
target_include_directories(antaralib PUBLIC  
    			   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    			   $<INSTALL_INTERFACE:include>)

target_sources(antaralib PRIVATE
                antara_base.h
                antara_queue.h
		serialization.h
		deserialization.h
		crc32.h)

install(FILES "include/ANTARA/Core" 
              "include/ANTARA/TCP" 
              "include/ANTARA/UDP" 
        #       "include/ANTARA/Serial" 
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ANTARA")

include(GNUInstallDirs)

install(TARGETS antaralib
	EXPORT antara-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


install(EXPORT antara-targets
        FILE antara-targets.cmake
        NAMESPACE ANTARA::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ANTARA")

