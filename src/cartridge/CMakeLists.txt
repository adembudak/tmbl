add_subdirectory(rom)
add_subdirectory(mbc1)

add_library(cartridge cartridge.cpp)
target_include_directories(cartridge PUBLIC ../../include)

target_link_libraries(cartridge PRIVATE mbc1 rom)
target_compile_features(cartridge INTERFACE cxx_std_17) # for the filesystemm support

