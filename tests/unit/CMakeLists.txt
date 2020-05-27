add_executable(reg8_test reg8_test.cpp)

target_include_directories(reg8_test PUBLIC ../../include)
target_link_directories(reg8_test PUBLIC ../../include)

target_link_libraries(reg8_test PUBLIC reg8 reg16 GTest::gtest_main)

gtest_discover_tests(
  reg8_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  TEST_LIST reg8_test)
