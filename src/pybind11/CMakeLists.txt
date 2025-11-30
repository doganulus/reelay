message(STATUS "Reelay version: ${PROJECT_VERSION}")
message(STATUS "Python version: ${PY_BUILD_CMAKE_PACKAGE_VERSION}")

# Make sure that the Python and CMake versions match)
if(DEFINED PY_BUILD_CMAKE_PACKAGE_VERSION)
  if(NOT "${PY_BUILD_CMAKE_PACKAGE_VERSION}" MATCHES
     "^${PROJECT_VERSION}(?:(a|b|rc)\d+)?(?:\.post\d+)?(?:\.dev\d+)?$")
    message(FATAL_ERROR "Version number does not match "
                        "(${PROJECT_VERSION} - ${PY_BUILD_CMAKE_PACKAGE_VERSION}).")
  endif()
endif()

# Find the Python development files
find_package(Python3 REQUIRED COMPONENTS Development.Module)

include(FetchContent)
fetchcontent_declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG v3.0.1)
fetchcontent_makeavailable(pybind11)

# Compile the example Python module
pybind11_add_module(_pybind11_module MODULE "main.cpp")
target_link_libraries(_pybind11_module PRIVATE pybind11::pybind11 reelay::reelay)
target_compile_definitions(
  _pybind11_module PRIVATE MODULE_NAME=$<TARGET_FILE_BASE_NAME:_pybind11_module>
                           VERSION_INFO="${PY_BUILD_CMAKE_PACKAGE_VERSION}")

# Hide all symbols by default (including external libraries on Linux)
set_target_properties(
  _pybind11_module
  PROPERTIES CXX_VISIBILITY_PRESET "hidden"
             VISIBILITY_INLINES_HIDDEN true
             RELEASE_POSTFIX ""
             DEBUG_POSTFIX ""
             RELWITHDEBINFO_POSTFIX ""
             MINSIZEREL_POSTFIX "")

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_options(_pybind11_module PRIVATE "LINKER:--exclude-libs,ALL")
endif()

# Install the Python module
install(
  TARGETS _pybind11_module
  EXCLUDE_FROM_ALL
  COMPONENT python_modules
  DESTINATION ${PY_BUILD_CMAKE_MODULE_NAME})
