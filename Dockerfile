FROM alpine AS builder

RUN apk add --no-cache git g++ make
RUN apk add --no-cache autoconf automake
RUN apk add --no-cache boost-dev

# Get Reelay
WORKDIR /home
RUN git clone https://github.com/doganulus/reelay --recurse-submodules
WORKDIR /home/reelay

# Build CUDD dependency
RUN make cudd
RUN make cudd-install

# Make the app
RUN g++ -std=c++14 -fPIC -pthread -O3 apps/ryjson1/*.cpp -o ryjson1 -Iinclude -lcudd

# Main image
FROM alpine

RUN apk add --no-cache libstdc++

COPY --from=builder /usr/local/lib/libcudd* /usr/local/lib/
COPY --from=builder /home/reelay/ryjson1 /usr/local/bin/