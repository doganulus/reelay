FROM ubuntu:18.04
RUN apt-get -qq update

# Install Dependecies
RUN apt-get install -qqy uuid-dev 
RUN apt-get install -qqy git 
RUN apt-get install -qqy gcc
RUN apt-get install -qqy g++
RUN apt-get install -qqy make
RUN apt-get install -qqy autotools-dev
RUN apt-get install -qqy libboost-all-dev 
RUN apt-get install -qqy libgmp-dev

# Build CUDD
WORKDIR /home
RUN git clone https://github.com/doganulus/cudd
WORKDIR /home/cudd
RUN make cudd
RUN make cudd-install
RUN ldconfig

# Install Reelay
WORKDIR /home
RUN git clone https://github.com/doganulus/reelay
WORKDIR /home/reelay
RUN make install

# Build Apps
RUN make apps
RUN make apps-install
WORKDIR /home

ENTRYPOINT /bin/bash