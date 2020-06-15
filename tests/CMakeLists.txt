find_package(GTest QUIET)

if(NOT GTest_FOUND)
  include(FetchContent)

  FetchContent_Declare( googletest
    QUIET
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0)

  FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(unit)
# add_subdirectory(rom)
