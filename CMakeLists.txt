cmake_minimum_required(VERSION 3.22)

file(READ "${CMAKE_SOURCE_DIR}/include/reelay/version.hpp" ver)

string(REGEX MATCH "REELAY_VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "REELAY_VERSION_MINOR ([0-9]*)" _ ${ver})
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

option(REELAY_BUILD_TESTS "Build the C++ unit tests" OFF)
option(REELAY_BUILD_APPS "Build Reelay Apps" OFF)
option(REELAY_BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

set(REELAY_BOOST_VERSION "1.89.0" CACHE STRING "Fetch Boost version")

include(GNUInstallDirs)
include(FetchContent)
include(cmake/CPM.cmake)

add_library(reelay INTERFACE)
add_library(reelay::reelay ALIAS reelay)

find_package(Threads REQUIRED)

CPMAddPackage(
  NAME Boost
  VERSION ${REELAY_BOOST_VERSION}
  URL https://github.com/boostorg/boost/releases/download/boost-${REELAY_BOOST_VERSION}/boost-${REELAY_BOOST_VERSION}-cmake.tar.gz
  OPTIONS
    "BOOST_USE_STATIC_LIBS=ON"
)

# set(Boost_USE_STATIC_LIBS TRUE)
# FetchContent_Declare(
#   Boost
#   URL https://github.com/boostorg/boost/releases/download/boost-${REELAY_BOOST_VERSION}/boost-${REELAY_BOOST_VERSION}-cmake.tar.gz
#   DOWNLOAD_EXTRACT_TIMESTAMP TRUE
# )
# FetchContent_MakeAvailable(Boost)

FetchContent_Declare(
  Cudd
  GIT_REPOSITORY https://github.com/cuddorg/cudd.git
  GIT_TAG        4.0.0
  SYSTEM
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(Cudd)

# reelay-core
target_include_directories(
  reelay INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(
  reelay INTERFACE
    Boost::icl
    Cudd::cudd
    Threads::Threads
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(
  TARGETS reelay
  EXPORT reelay-config
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

# export(
#   TARGETS reelay
#   NAMESPACE reelay::
#   FILE "${CMAKE_CURRENT_BINARY_DIR}/reelay-config.cmake")

# install(
#  EXPORT reelay-config
#  DESTINATION "${CMAKE_INSTALL_DATADIR}/reelay/cmake"
#  NAMESPACE reelay::
#)

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
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
