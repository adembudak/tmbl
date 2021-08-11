add_subdirectory(bus)
add_subdirectory(builtin)
add_subdirectory(cpu)
add_subdirectory(clock)
add_subdirectory(cartridge)
add_subdirectory(io)
add_subdirectory(ppu)
add_subdirectory(dma)
add_subdirectory(utils)

add_library(tmbl INTERFACE)
target_link_libraries(tmbl INTERFACE 
	bus builtin cpu clock cartridge io ppu dma utils)
target_compile_features(tmbl INTERFACE cxx_std_17)

add_library(tmbl::tmbl ALIAS tmbl)
