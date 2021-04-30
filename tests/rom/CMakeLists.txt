find_package(SDL2 REQUIRED)

set(test_executable test_runner)
add_executable(${test_executable} main.cpp)
target_link_directories(${test_executable} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_executable} PUBLIC tmbl::tmbl SDL2::SDL2 SDL2::SDL2main)
target_include_directories(${test_executable} PUBLIC ${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

if(BLARGG_TEST_ROMs)
  message(INFO "Not implemented yet")
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
