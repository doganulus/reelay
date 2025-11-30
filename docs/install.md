# Installation

Reelay is available both as a header-only C++ library and as a Python package with pre-built binary wheels, so you can choose the integration model that best fits your workflow.

## Python Package

### Quick Install

The easiest way to use Reelay is through the Python package. Pre-built wheels are available for Python `3.9` through `3.14`:

```bash
python -m pip install reelay
```

This will install the latest stable release from PyPI. All dependencies are bundled with the wheel, so no additional setup is required.

### Development Version

To install the latest development version directly from the repository:

```bash
python -m pip install git+https://github.com/doganulus/reelay.git
```

This command builds the extension module from source, which may take a few minutes. You'll need to satisfy C++ build dependencies on your system:

## C++ Library

Reelay is a header-only C++ library that can be integrated into your project through CMake.

### Requirements

- **C++ Standard**: C++17 or later
- **CMake**: Version 3.26 or newer
- **Dependencies**: Boost.ICL and CUDD

### Building from Source

Clone the repository and build:

```bash
git clone https://github.com/doganulus/reelay.git
cd reelay
cmake -B build
cmake --build build
```

Optional CMake configuration flags:

- `-DREELAY_BUILD_TESTS=ON` - Build C++ unit tests (default: OFF)
- `-DREELAY_BUILD_APPS=ON` - Build example applications (default: OFF)
- `-DREELAY_INSTALL=ON` - Enable installation (default: ON)

To install system-wide:

```bash
sudo cmake --install build
```

## Verification

### Python

Verify your Python installation:

```bash
python -c "import reelay; print(reelay.__version__)"
```

### C++

Create a simple test file:

```cpp
#include <reelay/monitors.hpp>
#include <iostream>

int main() {
    std::cout << "Reelay version: "
              << REELAY_VERSION_MAJOR << "."
              << REELAY_VERSION_MINOR << "."
              << REELAY_VERSION_PATCH << std::endl;
    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 test.cpp -o test -lreelay -lcudd
./test
```

## Containers

Pre-configured development containers are available in the `containers/devel` directory. See [Docker documentation](containers.md) for details.
