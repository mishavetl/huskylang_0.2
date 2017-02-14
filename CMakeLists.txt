cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(huskylang)

include(ExternalProject)

##
# Dependencies
##

if(DEFINED TESTS)
  ExternalProject_Add(
    Criterion

    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}

    GIT_REPOSITORY "https://github.com/Snaipe/Criterion.git"
    GIT_TAG "bleeding"

    INSTALL_COMMAND "echo"

    UPDATE_COMMAND ""
    PATCH_COMMAND ""
  )
endif(DEFINED TESTS)


##
# Config
##

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Version
set(VERSION_POSTFIX alpha)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

string(TIMESTAMP BUILD_VERSION "%y%m%d.%H%M%S")

set(PROGRAM_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${BUILD_VERSION}-${VERSION_POSTFIX}")

file(WRITE ${CMAKE_BINARY_DIR}/vsn "${PROGRAM_VERSION}")
configure_file(config.h.in config.h @ONLY)

# Standart
add_definitions(--std=gnu11)

# Warnings
add_definitions(-Wall -Wextra -pedantic -std=gnu11)

# Debug
add_definitions(-g)

if(DEFINED TESTS)
  ExternalProject_Get_Property(Criterion BINARY_DIR SOURCE_DIR)
  set(Criterion_BIN_DIR ${BINARY_DIR})
  set(Criterion_SRC_DIR ${SOURCE_DIR})
endif(DEFINED TESTS)

if(DEFINED TESTS)
  link_directories(${Criterion_BIN_DIR})
endif(DEFINED TESTS)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${Criterion_SRC_DIR}/include
  ${CMAKE_BINARY_DIR}
)

##
# Main Part
##

add_subdirectory(src)

add_executable(husky src/main.c)
target_link_libraries(husky src m)

##
# Tests
##

if(DEFINED TESTS)
  file(GLOB testfiles "tests/*.c")

  add_executable(husky_tests ${testfiles})
  add_executable(husky_list_tests "src/list/test.c")

  target_link_libraries(husky_tests src criterion)
  target_link_libraries(husky_list_tests src)
endif(DEFINED TESTS)
