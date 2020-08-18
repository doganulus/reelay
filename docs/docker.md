# Running Reelay in a Docker container

This document is for people who want to try Reelay apps using a Docker container. If you haven't Docker engine installed on your machice, please follow the original instructions [here](https://docs.docker.com/get-docker/).

When Docker engine is installed, pull the latest docker image of Reelay and run it in a container by typing:

```
docker pull doganulus/reelay
docker run -it doganulus/reelay
```

Then, inside the container, you can run the app `ryjson1` that provides a command-line interface for Reelay monitors that checks multi-line JSON logs against Rye specifications. For example,

```
ryjson1 -x '{p1} since[18:24] {p2}' examples.jsonl
```

Please also check `ryjson1 --help` for further information on other flags and general usage.