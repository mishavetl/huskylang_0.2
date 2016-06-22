cmake_minimum_required (VERSION 2.8)
project(huskylang)

include(ExternalProject)

##
# Dependencies
##

ExternalProject_Add(
  Criterion

  GIT_REPOSITORY "https://github.com/Snaipe/Criterion.git"
  GIT_TAG "bleeding"

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
)

##
# Config
##

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

##
# Main Part
##

add_subdirectory(src)

add_executable(husky src/main.c)
target_link_libraries(husky src)

##
# Tests
##

file(GLOB testfiles "tests/*.c")

add_executable(husky_tests ${testfiles})
target_link_libraries(husky_tests src criterion)
