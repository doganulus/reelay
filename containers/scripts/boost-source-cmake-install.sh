#!/usr/bin/env bash
#
# Example Usage:
# REELAY_INSTALL_PREFIX=/tmp/install REELAY_BOOST_VERSION=1.89.0 boost-source-cmake-install.sh

set -ex

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

# Ensure prerequisites are installed
if ! command -v cmake >/dev/null 2>&1; then
  echo "Error: 'cmake' is required but not installed. Please install cmake and try again." >&2
  exit 1
fi

# Set default values for Boost build configuration
REELAY_BOOST_VERSION="${REELAY_BOOST_VERSION:-"1.89.0"}"
REELAY_BOOST_CMAKE_RELEASE_URL="${REELAY_BOOST_CMAKE_RELEASE_URL:-"https://github.com/boostorg/boost/releases/download/boost-${REELAY_BOOST_VERSION}/boost-${REELAY_BOOST_VERSION}-cmake.tar.gz"}"
REELAY_BOOST_SOURCE_DIR="${REELAY_BOOST_SOURCE_DIR:-"/tmp/src/boost"}"
REELAY_BOOST_BUILD_DIR="${REELAY_BOOST_BUILD_DIR:-"/tmp/build/boost"}"
REELAY_BOOST_INSTALL_PREFIX="${REELAY_INSTALL_PREFIX:-"/usr/local"}"
REELAY_BOOST_INSTALL_LIBDIR="${REELAY_INSTALL_LIBDIR:-"lib"}"

# Get Boost CMake release tarball
mkdir -p "${REELAY_BOOST_SOURCE_DIR}"
curl -L "${REELAY_BOOST_CMAKE_RELEASE_URL}" | tar -xz --strip-components=1 -C "${REELAY_BOOST_SOURCE_DIR}"

# Build and install Boost
cd "${REELAY_BOOST_SOURCE_DIR}"

cmake \
  -S "${REELAY_BOOST_SOURCE_DIR}" \
  -B "${REELAY_BOOST_BUILD_DIR}" \
  -DBUILD_SHARED_LIBS=OFF \
  -DBOOST_ENABLE_MPI=OFF \
  -DBOOST_ENABLE_PYTHON=OFF \
  -DCMAKE_INSTALL_PREFIX="${REELAY_BOOST_INSTALL_PREFIX}" \
  -DCMAKE_INSTALL_LIBDIR="${REELAY_BOOST_INSTALL_LIBDIR}" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

cmake --build "${REELAY_BOOST_BUILD_DIR}" --target install -- -j"$(nproc)"

# Clean up temporary directory
echo "Cleaning up..."
rm -rf "${REELAY_BOOST_SOURCE_DIR}" "${REELAY_BOOST_BUILD_DIR}"

echo "Boost ${REELAY_BOOST_VERSION} installed successfully to ${REELAY_BOOST_INSTALL_PREFIX}."
