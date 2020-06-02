function(add_tombul_test test_name target)
  add_executable(${test_name} "${test_name}.cpp")

  target_include_directories(${test_name} PUBLIC ../../../include)
  target_link_directories(${test_name} PUBLIC ../../../include)

  target_link_libraries(${test_name} PUBLIC ${target} GTest::gtest_main)

  gtest_discover_tests(
    ${test_name}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    TEST_LIST ${test_name})
endfunction()

add_subdirectory(instructions)
add_subdirectory(registers)
