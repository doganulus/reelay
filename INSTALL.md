# Install 

Reelay solely tested on the latest Ubuntu LTS (18.04). A fresh Ubuntu installation at least requires the installation of following packages:

     sudo apt-get update
     sudo apt-get install -y cmake
     sudo apt-get install -y libboost-all-dev

Then, clone the `reelay` repository:
    
    git clone https://github.com/doganulus/reelay.git
    cd reelay

The following step is to build the expression parser module. This requires the installation of ANTLR4 parser generator. The following commands will download ANTLR4 from the source, build, and install ANTLR4 runtime library:

     make antlr4-runtime
     sudo make antlr4-runtime-install

Finally build and install `reelay` library, which contains the runtime parser module as follows:

     make 
     sudo make install

Note that you may need to update your shared library cache via `sudo ldconfig`.

Voila!

