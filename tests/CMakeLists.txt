find_package(GTest QUIET)

if(NOT GTest_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    googletest
    QUIET
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0)

  FetchContent_GetProperties(googletest)

  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR}
                     EXCLUDE_FROM_ALL)
  endif()

endif()

add_subdirectory(unit)
# add_subdirectory(rom)
