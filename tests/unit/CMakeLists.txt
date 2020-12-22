find_package(GTest)

function (add_tmbl_test source)
    add_executable(${source} ${source}.cpp)
    target_include_directories(${source} PUBLIC ../../include)
    target_link_directories(${source} PUBLIC ../../src)
	target_link_libraries(${source} PRIVATE cpu_internals GTest::gtest GTest::gtest_main) 
	add_test(${source} ${source})
endfunction()

add_tmbl_test(r8_test)
add_tmbl_test(r16_test)

