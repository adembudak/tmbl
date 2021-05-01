cmake_policy(SET CMP0110 NEW)

# test roms are fetched from: https://github.com/c-sp/gameboy-test-roms
set(test_roms_BASE_URL "https://adembudak.github.io/test_roms")
set(ExternalData_OBJECT_STORES ${CMAKE_CURRENT_SOURCE_DIR}/fetched_test_roms)
set(ExternalData_URL_TEMPLATES "${test_roms_BASE_URL}/%(algo)/%(hash)")

set(tmbl_TEST_ROM_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_roms)
set(blargg_roms ${tmbl_TEST_ROM_ROOT_DIRECTORY}/blargg)
set(cgb_acid2_rom ${tmbl_TEST_ROM_ROOT_DIRECTORY}/cgb-acid2)
set(dmg_acid2_rom ${tmbl_TEST_ROM_ROOT_DIRECTORY}/dmg-acid2)
set(gambatte_roms ${tmbl_TEST_ROM_ROOT_DIRECTORY}/gambatte)
set(mealybug_tearoom_test_roms ${tmbl_TEST_ROM_ROOT_DIRECTORY}/mealybug-tearoom-tests)
set(mooneye_gb_roms ${tmbl_TEST_ROM_ROOT_DIRECTORY}/mooneye-gb)

find_package(SDL2 REQUIRED)

set(test_executable test_runner)
add_executable(${test_executable} main.cpp)
target_link_directories(${test_executable} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_executable} PUBLIC tmbl::tmbl SDL2::SDL2 SDL2::SDL2main)
target_include_directories(${test_executable} PUBLIC ${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

function(register_test rom_dir)
  file(GLOB_RECURSE test_sha256_list LIST_DIRECTORIES TRUE ${rom_dir} ${rom_dir}/*.sha256)

  foreach(test ${test_sha256_list})
    if(IS_DIRECTORY ${test})
      cmake_path(RELATIVE_PATH test BASE_DIRECTORY ${tmbl_TEST_ROM_ROOT_DIRECTORY} OUTPUT_VARIABLE test)
	  list(APPEND ExternalData_URL_TEMPLATES "${test_roms_BASE_URL}/%(algo)/${test}/%(hash)")
    else()
      cmake_path(REMOVE_EXTENSION test LAST_ONLY OUTPUT_VARIABLE test_name)

      string(RANDOM dummy)
      externaldata_add_test( ${dummy}
        NAME ${test_name}
        COMMAND ${test_executable} DATA{${test_name}}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

      externaldata_add_target(${dummy} SHOW_PROGRESS ON)
    endif()
  endforeach()
endfunction()

if(BLARGG_TEST_ROMs)
  register_test(${blargg_roms})
endif()

if(CGB_ACID2_TEST_ROM)
  register_test(${cgb_acid2_rom})
endif()

if(DMG_ACID2_TEST_ROM)
  register_test(${dmg_acid2_rom})
endif()

if(GAMBATTE_TEST_ROMs)
  register_test(${gambatte_roms})
endif()

if(MEALYBUG_TEAROOM_TEST_ROMs)
  register_test(${mealybug_tearoom_test_roms})
endif()

if(MOONEYE_GB_TEST_ROMs)
  register_test(${mooneye_gb_roms})
endif()

