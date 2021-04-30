set(test_executable test_runner)

# todo: don't hard code path on template
# todo: upload roms on remote, your own website??

set(ExternalData_URL_TEMPLATES 
	"http://localhost:4000/test_roms/$(algo)/blargg/$(hash)") 

set(tmbl_TEST_ROM_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_roms)

find_package(SDL2 REQUIRED)

add_executable(${test_executable} main.cpp)
target_link_directories(${test_executable} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_executable} PUBLIC tmbl::tmbl SDL2::SDL2 SDL2::SDL2main)
target_include_directories(${test_executable} PUBLIC ${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# test roms are fetched from: https://github.com/c-sp/gameboy-test-roms

cmake_policy(SET CMP0110 NEW)
function(register_test rom_dir)
	file(GLOB_RECURSE test_sha256_list LIST_DIRECTORIES FALSE ${rom_dir} ${rom_dir}/*.sha256)

  foreach(test ${test_sha256_list})
    string(REPLACE ".sha256" "" test_name ${test}) # strip off .sha256 part
	string(RANDOM dummy)

	  externaldata_add_test(${dummy}
      NAME "test_${test_name}"
      COMMAND ${test_executable} DATA{"${test_name}"}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    externaldata_add_target(${dummy} SHOW_PROGRESS ON)
  endforeach()
endfunction()


if(BLARGG_TEST_ROMs)
  set(blargg_roms ${tmbl_TEST_ROM_ROOT_DIRECTORY}/blargg)
  register_test(${blargg_roms})
endif()

if(CGB_ACID2_TEST_ROMs)
  message(INFO "Not implemented yet")
endif()

if(DMG_ACID2_TEST_ROMs)
  message(INFO "Not implemented yet")
endif()

if(GAMBATTE_TEST_ROMs)
  message(INFO "Not implemented yet")
endif()

if(MEALYBUG_TEAROOM_TEST_ROMs)
  message(INFO "Not implemented yet")
endif()

if(MOONEYE_GB_TEST_ROMs)
  message(INFO "Not implemented yet")
endif()
