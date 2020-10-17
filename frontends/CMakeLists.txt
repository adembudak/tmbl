add_library(gameboy gameboy.cpp "sdl/main.cpp")
target_include_directories(gameboy PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(gameboy PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(gameboy PUBLIC tmbl::tmbl)

add_subdirectory(sdl)