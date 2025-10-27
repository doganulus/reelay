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

option(BUILD_TESTS "Build the C++ unit tests" ON)
option(BUILD_REELAY_APPS "Build Reelay Apps" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

include(GNUInstallDirs)

add_library(reelay INTERFACE)
add_library(reelay::reelay ALIAS reelay)

find_package(Threads REQUIRED)
find_package(Boost 1.82.0 REQUIRED CONFIG)

include(FetchContent)
FetchContent_Declare(
  cudd
  GIT_REPOSITORY https://github.com/cuddorg/cudd.git
  GIT_TAG        4.0.0
  SYSTEM
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(cudd)

# Define a target for CUDD manually (if it doesn't export one)
if(NOT TARGET cudd)
  add_library(cudd STATIC IMPORTED GLOBAL)
  # Assume it’s built under cudd_SOURCE_DIR
  set_target_properties(cudd PROPERTIES
    IMPORTED_LOCATION ${cudd_BINARY_DIR}/libcudd.a
    INTERFACE_INCLUDE_DIRECTORIES
      "${cudd_SOURCE_DIR}/cudd;${cudd_SOURCE_DIR}/cplusplus"
  )
endif()

# reelay-core
target_include_directories(
  reelay INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
                   $<BUILD_INTERFACE:${cudd_SOURCE_DIR}/include/cudd/>
                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(reelay INTERFACE Threads::Threads cudd::cudd)

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

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(BUILD_REELAY_APPS)
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
