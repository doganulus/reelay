FROM ubuntu:18.04

RUN apt-get -qq update && apt-get -qqy install sudo
# Prereqs
RUN sudo apt-get install -qqy uuid-dev git g++ make libboost-all-dev 
# Clone reelay
RUN git clone https://github.com/doganulus/reelay
WORKDIR reelay
# install reelay
RUN make install
# install reelay apps
RUN make apps
RUN make apps-install
WORKDIR ..

ENTRYPOINT /bin/bash