# Running Reelay in a Docker container
This document is designed for people who want to run `reelay` apps without installing all the dependencies on your machine. 

First, pull the latest docker image and run it in a container by typing:

    docker pull doganulus/reelay
    docker run -it doganulus/reelay

Then you can run `reelay` apps inside the container. For example, 

    rymtl '{p1} since[18:24] {p2}' reelay/apps/mtl/examples/2.csv --verbose 1

Note that you can install Docker engine on Ubuntu by following instructions from [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/) if you don't have it already.