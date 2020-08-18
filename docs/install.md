# Install

## Dependencies

### 1) C++11 Compiler (for C++ or Python users)

Reelay requires a C++ compiler compliant with C++11 standard. Today all major compiler frameworks already provide full C++11 support. We mainly use `gcc` and sometimes `clang` for additional analyses (e.g. extra warnings).

### 2) Boost C++ Libraries (for C++ or Python users)

Reelay requires Boost C++ libraries at several places. In particular, we heavily use Boost Interval Container Library (ICL) inside timed monitors. Below is a list of how to install Boost for several environments:

For Debian/Ubuntu:

    apt install -y libboost-all-dev

For CentOS/Fedora:

    yum install -y boost-devel

For Arch/Manjaro:

    pacman -S boost

For MacOS (assuming homebrew installed):

    brew install boost

Note that we mainly develop Reelay on Ubuntu 20.04 therefore it's the most tested environment among these environments. For Windows machines, we suggest installing Reelay on Ubuntu on Windows Subsystem for Linux (WSL).

### 3) CUDD Library (only for C++ users)

Reelay data monitors additionally require CUDD binary decision diagram (BDD) library. If CUDD library is not installed on your system, please see the next section how to build CUDD from the source.

### 4) Pybind11 (only for Python users)

To use Reelay in Python, we additionaly require the package `pybind11` that provides seamless operability between C++11 and Python.

    python -m pip install pybind11 

This command also install `pybind11` package and required C++ header files such as `pybind11/pybind11.h` and `pybind11/stl.h`.

## Install Reelay C++ Library

This section describe the installation steps on Ubuntu 18.04.

First, clone the `reelay` repository and its submodules using `--recursive` flag as follows:

    git clone https://github.com/doganulus/reelay.git --recurse-submodules
    cd reelay

Since `reelay` is a header-only library, you only need to copy the header files into your include directory using the command:

    sudo make install

Finally you can build and install CUDD library by typing:

    make cudd
    sudo make cudd-install

These commands will build the shared library `libcudd.so` and place it under `/usr/local/lib` directory by default. Programs that use Reelay data monitors have to link this library at the compilation.

<!-- ## Install Reelay Apps

The following command will compile and install command-line applications using `reelay` library to monitor formal specifications over CSV files:

    make apps
    sudo make apps-install

Currently available applications are `rymtl` for past MTL specifications, `rystl` for past STL specifications, and `ryjavu` for past QTL specifications. Note that `C++17` support is needed to build Reelay apps. -->

## Install Reelay Python Package

A set of predefined Reelay monitors can be accessed from Python. The following command will install the `reelay` package in your Python environment:

    python -m pip install reelay

The command above will install latest release of Reelay.  Here `python` executable could be any python3 executable as Reelay is tested from `3.5` to the latest `3.8`.

Alternatively you can install the development version of Reelay from the master branch directly.

    python -m pip install git+https://github.com/doganulus/reelay.git

This command will build the extension module from the source therefore it may take longer to install. Finally Python installation handles the CUDD dependency automatically and you do not need to install CUDD if you are only interested in using Reelay from Python.
