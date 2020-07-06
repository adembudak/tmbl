set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;-p=${CMAKE_BINARY_DIR}/compile_commands.json;-checks=clang-diagnostic-*,clang-analyzer-*,-*,-abseil-*,-android-*,-boost-*,bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-*,-darwin-*,-google-*,hicpp-*,-linuxkernel-*,-llvm-*,-llvmlibc-*,misc-*,-modernize-*,-mpi-*,-objc-*,-openmp-*,performance-*,portability-*,-readability-*,-zircon-*,-cppcoreguidelines-avoid-magic-numbers
)
