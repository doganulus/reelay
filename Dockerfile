FROM ubuntu:18.04

RUN apt-get -qq update
# Prereqs
RUN apt-get install -qqy uuid-dev 
RUN apt-get install -qqy git 
RUN apt-get install -qqy g++
RUN apt-get install -qqy make 
RUN apt-get install -qqy libboost-all-dev 
RUN apt-get install -qqy libgmp-dev
# Clone reelay
WORKDIR /home
RUN git clone https://github.com/doganulus/reelay
WORKDIR /home/reelay
# install reelay
RUN make install
# install reelay apps
RUN make apps
RUN make apps-install
WORKDIR /home

ENTRYPOINT /bin/bash