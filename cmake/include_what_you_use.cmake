find_program(include-what-you-use iwyu_tool.py)

if(include-what-you-use-NOTFOUND)
  message(FATAL_ERROR "include-what-you-use not fount")
endif()

execute_process(COMMAND ${include-what-you-use} -j 4 -p
                        ${CMAKE_BINARY_DIR}/compile_commands.json)
