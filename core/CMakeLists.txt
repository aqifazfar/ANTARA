
target_include_directories(antaralib PUBLIC  
    					   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    					   $<INSTALL_INTERFACE:include>)

target_sources(antaralib PRIVATE
				Antara.cpp	 
				antara_serial.cpp
				antara_serialization.cpp 
				antara_tcp.cpp
				antara_udp.cpp
			    base64.cpp
				crc32.cpp)

install(FILES "include/ANTARA/Antara.h" 
DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ANTARA")

include(GNUInstallDirs)

install(TARGETS antaralib
		EXPORT BUMANTARASDKTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT BUMANTARASDKTargets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/BUMANTARASDK"
        NAMESPACE BUMANTARASDK::
        FILE BUMANTARASDKTargets.cmake)
