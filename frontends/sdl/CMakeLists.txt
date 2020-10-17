add_executable(sdl_frontend main.cpp)
target_link_directories(sdl_frontend PUBLIC ..)
target_link_libraries(sdl_frontend PUBLIC gameboy)