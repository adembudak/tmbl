find_package(GTest)

function(add_tmbl_executable name)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} PUBLIC ../../include)
    target_link_directories(${name} PUBLIC ../../src)
    target_link_libraries(${name} PRIVATE ${ARGN})
endfunction()

function (add_tmbl_test src)
   target_link_libraries(${src} PRIVATE GTest::gtest_main)
   add_test(NAME ${src} COMMAND ${src})
endfunction()

add_tmbl_executable(r8_test cpu_internals)
add_tmbl_test(r8_test)

add_tmbl_executable(r16_test cpu_internals)
add_tmbl_test(r16_test)

add_tmbl_executable(n8_test cpu_internals)
add_tmbl_test(n8_test)

add_tmbl_executable(n16_test cpu_internals)
add_tmbl_test(n16_test)

add_tmbl_executable(e8_test cpu_internals)
add_tmbl_test(e8_test)

add_tmbl_executable(flags_test cpu_internals)
add_tmbl_test(flags_test)

