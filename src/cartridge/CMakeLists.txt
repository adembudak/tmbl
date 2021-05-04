add_subdirectory(type)
add_subdirectory(metadata)

add_library(cartridge cartridge.cpp)
target_include_directories(cartridge PUBLIC ../../include)

target_link_libraries(cartridge PRIVATE mbc1 mbc2 rom)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_link_libraries(cartridge PRIVATE pakInfo)
	target_compile_definitions(cartridge PUBLIC PRINT_PAK_INFO)
endif()

