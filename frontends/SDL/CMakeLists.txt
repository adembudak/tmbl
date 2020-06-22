find_package(sdl2 QUIET REQUIRED)

add_executable(tombul main.cpp)
target_link_libraries(tombul PUBLIC SDL2::SDL2 Gameboy::Gameboy)
target_include_directories(tombul PUBLIC ../../include)
