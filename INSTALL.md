Reelay has a few dependecies that needs to be installed before the use.

First, install ANTLR4 C++ Runtime (this also requires CMake):
    
     git clone https://github.com/antlr/antlr4.git
     cd antlr4/runtime/Cpp
     mkdir build && mkdir run && cd build
     cmake .. 
     make
     sudo make install

Please refer to (the original documentation)[https://github.com/antlr/antlr4/tree/master/runtime/Cpp] for any problem.

Then, install reelay/monitors library as follows:

     git clone https://github.com/doganulus/monitors.git
     make
     sudo make install

Finally, build some applications for a head start:

	 make apps
	 sudo make apps-install

The latter would build executables to monitor temporal logic formulas over CSV files.

