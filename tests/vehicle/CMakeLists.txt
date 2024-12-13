add_executable(vehicle vehicle.cpp)

find_package(ANTARA REQUIRED)

target_link_libraries(vehicle PRIVATE ANTARA::antaralib)