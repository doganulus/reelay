# Running Reelay in a Docker container
This document is designed for people that want to run `reelay` without needing to install all dependencies. First, pull the latest docker image as follws:

    docker pull doganulus/reelay
    docker run -it doganulus/reelay

Then you can run `reelay` apps inside the container. For example, 

    rymtl 'p1 since[18:24] p2' reelay/apps/mtl/examples/2.csv --verbose 1

Note that you can install Docker CE on Ubuntu [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/) if you don't have it already.