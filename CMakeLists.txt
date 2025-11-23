cmake_minimum_required(VERSION 3.26)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
             "Debug"
             "Release"
             "MinSizeRel"
             "RelWithDebInfo")
else()
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/reelay/version.hpp" version_file)

string(REGEX MATCH "REELAY_VERSION_MAJOR ([0-9]*)" _ ${version_file})
set(version_major ${CMAKE_MATCH_1})

string(REGEX MATCH "REELAY_VERSION_MINOR ([0-9]*)" _ ${version_file})
set(version_minor ${CMAKE_MATCH_1})

string(REGEX MATCH "REELAY_VERSION_PATCH ([0-9]*)" _ ${version_file})
set(version_patch ${CMAKE_MATCH_1})

project(
  "reelay"
  LANGUAGES CXX
  VERSION "${version_major}.${version_minor}.${version_patch}")
message(STATUS "Building Reelay version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 --coverage")

option(REELAY_BUILD_TESTS "Build the C++ unit tests" OFF)
option(REELAY_BUILD_APPS "Build Reelay Apps" OFF)
option(REELAY_BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

include(GNUInstallDirs)
include(FetchContent)

add_library(reelay INTERFACE)
add_library(reelay::reelay ALIAS reelay)

# Dependencies
find_package(Threads REQUIRED)

set(BOOST_INCLUDE_LIBRARIES icl)
set(BOOST_USE_STATIC_LIBS TRUE)
fetchcontent_declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz
      DOWNLOAD_EXTRACT_TIMESTAMP
      TRUE
      FIND_PACKAGE_ARGS
      1.82.0
      CONFIG)
fetchcontent_makeavailable(Boost)

set(CUDD_INSTALL
    OFF
    CACHE BOOL "" FORCE)
fetchcontent_declare(
  cudd
  GIT_REPOSITORY https://github.com/cuddorg/cudd.git
  GIT_TAG 4.0.0
  FIND_PACKAGE_ARGS 4.0.0 CONFIG)
fetchcontent_makeavailable(cudd)

# reelay-core
target_include_directories(
  reelay INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(reelay INTERFACE Boost::icl cudd::cudd Threads::Threads)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(
  TARGETS reelay
  EXPORT reelayConfig
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# install( EXPORT reelayConfig NAMESPACE reelay:: DESTINATION
# ${CMAKE_INSTALL_LIBDIR}/cmake/reelay )

add_subdirectory(src)

if(REELAY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(REELAY_BUILD_APPS)
  message(STATUS "Building Reelay apps...")
  add_subdirectory(apps)
endif()

if(PROJECT_IS_TOP_LEVEL AND UNIX)
  # Create symlink to compile_commands.json on the project directory
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
