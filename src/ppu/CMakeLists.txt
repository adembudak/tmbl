add_library(ppu ppu.cpp)
target_link_libraries(ppu PRIVATE ppu_internals)
target_include_directories(ppu PUBLIC ../../include)

add_subdirectory(registers)
