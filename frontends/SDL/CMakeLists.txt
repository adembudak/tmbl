
find_library(sdl2 QUIET REQUIRED)

add_executable(tombul main.cpp)

target_include_directories(tombul PUBLIC ../../include)
target_link_libraries(tombul PRIVATE Gameboy::Gameboy)
target_link_libraries(tombul PRIVATE ${SDL2_LIBRARIES})

