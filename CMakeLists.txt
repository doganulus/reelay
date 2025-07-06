cmake_minimum_required(VERSION 3.22)

# Read version numbers from version.hpp
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/reelay/version.hpp" ver)

string(REGEX MATCH "#define[ \t]+REELAY_VERSION_MAJOR[ \t]+([0-9]+)" _ "${ver}")
set(ver_major "${CMAKE_MATCH_1}")

string(REGEX MATCH "#define[ \t]+REELAY_VERSION_MINOR[ \t]+([0-9]+)" _ "${ver}")
set(ver_minor "${CMAKE_MATCH_1}")

project(
  reelay
  VERSION "${ver_major}.${ver_minor}"
  LANGUAGES CXX)
message(STATUS "Reelay version: ${PROJECT_VERSION}")

# C++ build settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optional: coverage flags (used only in special configs)
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 --coverage")

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build." FORCE)
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

# Build options
option(BUILD_TESTS "Build the C++ unit tests" ON)
option(BUILD_REELAY_APPS "Build Reelay Apps" ON)
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

# Install helpers
include(GNUInstallDirs)

# Create the main interface library
add_library(reelay INTERFACE)
add_library(reelay::reelay ALIAS reelay)

# Header and link dependencies
target_include_directories(
  reelay INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Required dependencies
find_package(Boost 1.82.0 REQUIRED)

find_library(cudd_static NAMES libcudd.a REQUIRED)
target_link_libraries(reelay INTERFACE ${cudd_static})

# Install headers
install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Install targets
install(TARGETS reelay EXPORT reelayTargets)

# Export config files
install(
  EXPORT reelayTargets
  NAMESPACE reelay::
  FILE reelayTargets.cmake
  DESTINATION "${CMAKE_INSTALL_DATADIR}/reelay/cmake")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/reelayConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/reelayConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/reelayConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/reelay/cmake")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/reelayConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/reelayConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/reelay/cmake")

# Add sources and tests
add_subdirectory(src)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(BUILD_REELAY_APPS)
  message(STATUS "Building Reelay apps...")
  add_subdirectory(apps)
endif()
