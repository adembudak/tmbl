find_package(GTest QUIET REQUIRED)

include_directories(../include)

add_executable(reg8_test reg8_test.cpp)

target_link_libraries(reg8_test PUBLIC 
	#	GTest::gtest 
		GTest::gtest_main
	#	GTest::gmock
	#	GTest::gmock_main
	)

gtest_discover_tests(reg8_test
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	TEST_LIST reg8_test)

