add_executable(antara-cli antara-cli.cpp)

target_include_directories(antara-cli PRIVATE ${CMAKE_SOURCE_DIR}/lib/cxxopts/include)

add_custom_command(TARGET antara-cli POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:antara-cli> ${CMAKE_BINARY_DIR}/
)
