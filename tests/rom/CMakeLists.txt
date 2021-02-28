find_package(SDL2 REQUIRED)

add_executable(rom_tests main.cpp)
target_link_directories(rom_tests PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(rom_tests PUBLIC tmbl::tmbl SDL2::SDL2 SDL2::SDL2main)
target_include_directories(rom_tests PUBLIC ${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

set(GAMEBOY_TEST_ROMS_URL "https://github.com/c-sp/gameboy-test-roms/releases/download/v2.0/gameboy-test-roms-v2.0.zip")

externalproject_add(
  public_domain_roms
  URL ${GAMEBOY_TEST_ROMS_URL}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test_roms
  BUILD_IN_SOURCE NO
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "")

#[[
file(GLOB_RECURSE result LIST_DIRECTORIES YES
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/test_roms/blargg" *.gb)

foreach(rom ${result})
    add_test(NAME "rom_tests_${rom}"
	    COMMAND rom_tests ${rom}
	    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
]]
#
