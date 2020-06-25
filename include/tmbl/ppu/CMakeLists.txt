add_library(ppu ppu.cpp)
target_include_directories(ppu PRIVATE .)
target_include_directories(ppu PUBLIC ../cpu)
