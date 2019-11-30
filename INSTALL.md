# Install 

Reelay is tested on the latest Ubuntu LTS (18.04). A fresh Ubuntu installation at least requires the installation of following packages:

     sudo apt-get update
     sudo apt-get install -y libboost-all-dev

Then, clone the `reelay` repository:
    
    git clone https://github.com/doganulus/reelay.git
    cd reelay

Since `reelay` is a header-only library, you only need to copy the header files into your include directory using the command:

    sudo make install

Now you can include `reelay` monitors in your C++ project!

    #include "reelay/monitors.hpp"

Please also check the apps folder for more information on the usage.

## Standalone command line apps

The following command will compile and install command-line applications using `reelay` library to monitor formal specifications over CSV files: 

    make apps
    sudo make apps-install

Currently available applications are `rymtl` for past MTL specifications and `rystl` for past STL specifications.

## Python package

A set of predefined Reelay monitors can be accessed from Python. The following command will install the `reelay` package in your Python environment:

    make python

Currently past LTL, past MTL, and past STL monitors are available from Python over discrete and dense time signals. Please check [example python files](https://github.com/doganulus/reelay/tree/master/python/examples).