FROM ubuntu:18.04

RUN apt-get -qq update && apt-get -qqy install sudo
# Prereqs
RUN sudo apt-get install -qqy uuid-dev curl git g++ make cmake libboost-all-dev  default-jre
# Clone reelay
RUN git clone https://github.com/doganulus/reelay
WORKDIR reelay
# install antlr4
RUN make antlr4-runtime
RUN make antlr4-runtime-install
# install reelay
RUN make
RUN make install
# install reelay apps
RUN make apps
RUN make apps-install
WORKDIR ..

RUN ldconfig
ENTRYPOINT /bin/bash