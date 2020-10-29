add_library(gameboy_test gameboy.cpp)
target_include_directories(gameboy_test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(gameboy_test PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(gameboy_test PUBLIC tmbl::tmbl)

add_executable(rom_tests main.cpp)
target_link_libraries(rom_tests PUBLIC gameboy_test)

file(GLOB_RECURSE result LIST_DIRECTORIES YES
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/test_roms/blargg" *.gb)


foreach(rom ${result})
    add_test(NAME "rom_tests_${rom}"
	    COMMAND rom_tests ${rom}
	    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
