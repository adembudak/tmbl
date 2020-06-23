add_subdirectory(mbc)

add_library(cartridge cartridge.cpp)
target_include_directories(cartridge PUBLIC .)

target_link_libraries(cartridge PRIVATE mbc0)
