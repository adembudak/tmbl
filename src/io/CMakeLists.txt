#add_subdirectory(apu)
add_subdirectory(interrupts)
add_subdirectory(joypad)
#add_subdirectory(serial)

add_library(registers registers.cpp)
target_include_directories(registers PUBLIC ${tmbl_SOURCE_DIR}/include)

add_library(io INTERFACE)
target_link_libraries(io INTERFACE registers interrupts joypad)
