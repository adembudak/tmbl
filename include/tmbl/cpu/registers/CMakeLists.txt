
add_library(reg8 reg8.cpp)
add_library(reg16 reg16)

target_include_directories(reg8 PUBLIC .)
target_include_directories(reg16 PUBLIC .)
