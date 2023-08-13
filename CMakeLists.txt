cmake_minimum_required(VERSION 3.22)

file(READ "${CMAKE_SOURCE_DIR}/include/reelay/version.hpp" ver)

string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})

project(
  "reelay"
  LANGUAGES CXX
  VERSION "${ver_major}.${ver_minor}")
message(STATUS "Reelay version: ${ver_major}.${ver_minor}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 --coverage")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

option(BUILD_TESTS ON)
option(BUILD_REELAY_APPS ON)
option(BUILD_PYTHON_BINDINGS OFF)

include(GNUInstallDirs)

add_library(reelay INTERFACE)
add_library(reelay::reelay ALIAS reelay)

find_package(
  Boost 1.82.0 REQUIRED
  # COMPONENTS json
  )

# find_library( boost_json_static NAMES libboost_json.a REQUIRED NO_CACHE )
find_library(cudd_static NAMES libcudd.a REQUIRED NO_CACHE)

# reelay-core
target_include_directories(
  reelay INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>/include)

target_link_libraries(reelay INTERFACE ${cudd_static})

install(TARGETS reelay LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

add_subdirectory(src)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(BUILD_REELAY_APPS)
  message(STATUS "Building Reelay apps...")
  add_subdirectory(apps)
endif()
