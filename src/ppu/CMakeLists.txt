add_subdirectory(internals)

add_library(ppu ppu.cpp)
target_link_libraries(ppu PRIVATE ppu_internals)
target_include_directories(ppu PUBLIC ${tmbl_SOURCE_DIR}/include)

