add_executable(antara-cli antara-cli.cpp)

target_link_libraries(antara-cli PRIVATE libusb)

include(GNUInstallDirs)

install(TARGETS antara-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})