#add_subdirectory(apu)
add_subdirectory(interrupts)
#add_subdirectory(joypad)
#add_subdirectory(serial)
add_subdirectory(timer)

add_library(registers registers.cpp)
target_include_directories(registers PUBLIC ../../include)
