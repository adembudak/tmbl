find_package(GTest QUIET REQUIRED)

function(add_tmbl_executable name)
  add_executable(${name} ${name}.cpp)
  target_include_directories(${name} PUBLIC ${tmbl_SOURCE_DIR}/include)
  target_link_directories(${name} PUBLIC ${tmbl_SOURCE_DIR}/src)
  target_link_libraries(${name} PRIVATE ${ARGN})
endfunction()

function(add_tmbl_test src)
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

add_tmbl_executable(rom_test rom)
add_tmbl_test(rom_test)

add_tmbl_executable(mbc1_test mbc1)
add_tmbl_test(mbc1_test)

add_tmbl_executable(mbc2_test mbc2)
add_tmbl_test(mbc2_test)

add_tmbl_executable(mbc5_test mbc5)
add_tmbl_test(mbc5_test)

add_tmbl_executable(intr_test interrupts)
add_tmbl_test(intr_test)
