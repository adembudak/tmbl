
add_library(lcdc lcdc.cpp)
target_include_directories(lcdc PRIVATE ../../../include)

add_library(stat stat.cpp)
target_include_directories(stat PRIVATE ../../../include)

add_library(bgp bgp.cpp)
target_include_directories(bgp PRIVATE ../../../include)
