cmake_minimum_required(VERSION 3.22)

project("reelay" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 --coverage")

set(ENABLE_TESTS ON)
set(BUILD_REELAY_APPS ON)

include(GNUInstallDirs)

add_library(reelay INTERFACE)
add_library(reelay::reelay ALIAS reelay)

find_library(boost_json_static NAMES libboost_json.a REQUIRED NO_CACHE)
find_library(cudd_static NAMES libcudd.a REQUIRED NO_CACHE)

# reelay-core
target_include_directories(
  reelay INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>/include)

target_link_libraries(reelay INTERFACE ${cudd_static})

install(TARGETS reelay LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

if(ENABLE_TESTS)
  add_subdirectory(tests)
  enable_testing()
endif()

if(BUILD_REELAY_APPS)
  message(STATUS "Building Reelay apps...")
  add_subdirectory(apps)
endif()
