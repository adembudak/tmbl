add_library(bootrom INTERFACE)
target_sources(bootrom INTERFACE bootrom.h)
target_include_directories(bootrom INTERFACE .)
