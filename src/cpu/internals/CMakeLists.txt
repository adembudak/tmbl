add_library(cpu_internals flags.cpp n16.cpp 
	n8.cpp e8.cpp r16.cpp r8.cpp)
target_include_directories(cpu_internals PRIVATE ../../../include)
