add_library(antaralib)
add_library(BUMANTARASDK::antaralib ALIAS antaralib)
	
if(MSVC)
  target_compile_options(antaralib PRIVATE /W4 /WX)
else()
  target_compile_options(antaralib PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(core)
