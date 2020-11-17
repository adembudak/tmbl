add_subdirectory(internals)
add_subdirectory(data)

add_library(ppu ppu.cpp)
target_link_libraries(ppu PRIVATE ppu_internals ppu_data)
target_include_directories(ppu PUBLIC ../../include)

