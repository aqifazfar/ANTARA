add_executable(antara-cli antara-cli.cpp)

target_include_directories(antara-cli PRIVATE ${CMAKE_SOURCE_DIR}/lib/cxxopts/include ${LIBUSB_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

target_link_libraries(antara-cli PRIVATE ${LIBUSB_LIBRARIES})

add_custom_command(TARGET antara-cli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:antara-cli> ${CMAKE_BINARY_DIR}/
)
