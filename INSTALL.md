# Install 

Reelay solely tested on the latest Ubuntu LTS (18.04). A fresh Ubuntu installation at least requires the installation of following packages:

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

