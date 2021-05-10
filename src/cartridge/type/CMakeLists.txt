add_library(mbc1 mbc1.cpp)
target_include_directories(mbc1 PRIVATE ${tmbl_SOURCE_DIR}/include)

add_library(mbc2 mbc2.cpp)
target_include_directories(mbc2 PRIVATE ${tmbl_SOURCE_DIR}/include)

add_library(rom rom.cpp)
target_include_directories(rom PRIVATE ${tmbl_SOURCE_DIR}/include)


