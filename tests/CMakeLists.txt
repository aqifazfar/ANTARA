add_executable(test test.cpp)

find_package(ANTARA REQUIRED)

target_link_libraries(test PRIVATE ANTARA::antaralib)

if(MSVC)
  target_compile_options(test PRIVATE /W4 /WX)
else()
  target_compile_options(test PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-exceptions)
endif()