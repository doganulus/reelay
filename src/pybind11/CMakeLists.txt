set(PY_VERSION_SUFFIX "")
set(PY_FULL_VERSION ${PROJECT_VERSION}${PY_VERSION_SUFFIX})

message(STATUS "Python version: ${PY_FULL_VERSION}")

# Make sure that the Python and CMake versions match
if(DEFINED PY_BUILD_CMAKE_PACKAGE_VERSION)
  if(NOT
     "${PY_BUILD_CMAKE_PACKAGE_VERSION}"
     MATCHES
     "^${PY_FULL_VERSION}$"
  )
    message(FATAL_ERROR
              "Version number does not match "
              "(${PY_BUILD_CMAKE_PACKAGE_VERSION} - ${PY_FULL_VERSION})."
    )
  endif()
endif()

include(cmake/QueryPythonForPybind11.cmake)
find_pybind11_python_first()

# Find the Python development files
find_package(Python3 REQUIRED COMPONENTS Development.Module)

# pybind11 is header-only, so finding a native version is fine
find_package(
  pybind11
  ${ARGN}
  REQUIRED
  CONFIG
  CMAKE_FIND_ROOT_PATH_BOTH
)

# Compile the example Python module
pybind11_add_module(
  _pybind11_module
  MODULE
  "main.cpp"
)
target_link_libraries(
  _pybind11_module PRIVATE pybind11::pybind11 reelay::reelay
)
target_compile_definitions(
  _pybind11_module PRIVATE MODULE_NAME=$<TARGET_FILE_BASE_NAME:_pybind11_module>
                           VERSION_INFO="${PY_FULL_VERSION}"
)

# Hide all symbols by default (including external libraries on Linux)
set_target_properties(
  _pybind11_module
  PROPERTIES
    CXX_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN true
    RELEASE_POSTFIX ""
    DEBUG_POSTFIX ""
    RELWITHDEBINFO_POSTFIX ""
    MINSIZEREL_POSTFIX ""
)

if(CMAKE_SYSTEM_NAME
   MATCHES
   "Linux"
)
  target_link_options(
    _pybind11_module
    PRIVATE
    "LINKER:--exclude-libs,ALL"
  )
endif()

# Install the Python module
install(TARGETS _pybind11_module
        EXCLUDE_FROM_ALL
        COMPONENT python_modules
        DESTINATION ${PY_BUILD_CMAKE_MODULE_NAME}
)
