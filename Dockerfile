FROM ubuntu:18.04

RUN apt-get update
# Prereqs
RUN apt-get install -qqy --no-install-recommends uuid-dev 
RUN apt-get install -qqy --no-install-recommends git 
RUN apt-get install -qqy --no-install-recommends g++
RUN apt-get install -qqy --no-install-recommends make 
RUN apt-get install -qqy --no-install-recommends libboost-all-dev 
RUN apt-get install -qqy --no-install-recommends libgmp-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
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