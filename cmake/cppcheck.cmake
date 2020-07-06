set(CMAKE_CXX_CPPCHECK
    cppcheck;--enable=all;--suppress=missingIncludeSystem;--project=${CMAKE_BINARY_DIR}/compile_commands.json;-itests
)
