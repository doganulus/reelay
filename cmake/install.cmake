include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
  TARGETS reelay
  EXPORT reelayTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/reelay
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install reelay header files
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/reelay
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp")

install(
  EXPORT reelayTargets
  NAMESPACE reelay::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/reelay)

# -------------------------------
# Version and Config files
# -------------------------------
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/reelay")

# Version file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/reelayConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/reelayConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/reelayConfig.cmake"
  INSTALL_DESTINATION ${CONFIG_INSTALL_DIR})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/reelayConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/reelayConfigVersion.cmake"
        DESTINATION ${CONFIG_INSTALL_DIR})
