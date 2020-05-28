function (register_test test_name)
  add_executable(${test_name} "${test_name}.cpp")

  target_include_directories(${test_name} PUBLIC ../../include)
  target_link_directories(${test_name} PUBLIC ../../include)

  target_link_libraries(${test_name} PUBLIC reg8 reg16 GTest::gtest_main)

  gtest_discover_tests(
  ${test_name} 
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  TEST_LIST ${test_name})
endfunction()

register_test(reg8_test)
register_test(reg16_test)
