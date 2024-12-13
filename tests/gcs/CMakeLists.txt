add_executable(gcs gcs.cpp)

find_package(ANTARA REQUIRED)

target_link_libraries(gcs PRIVATE ANTARA::antaralib)