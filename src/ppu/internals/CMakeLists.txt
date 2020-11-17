
add_library(ppu_internals lcdc.cpp stat.cpp bgp.cpp obp.cpp bcpd.cpp bcps.cpp ocpd.cpp ocps.cpp)
target_include_directories(ppu_internals PRIVATE ../../../include)
