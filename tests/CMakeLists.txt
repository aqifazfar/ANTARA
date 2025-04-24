add_executable(test test.cpp)

find_package(ANTARA REQUIRED)

target_link_libraries(test PRIVATE ANTARA::antaralib)