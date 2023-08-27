FROM ghcr.io/doganulus/reelay-devel as builder-reelay

ARG VERSION=main
ARG REELAY_VERSION=${VERSION}

RUN git clone https://github.com/doganulus/reelay.git /tmp/reelay \
      --depth 1 \
      --branch ${REELAY_VERSION} \
    && cmake -S/tmp/reelay -B/tmp/reelay/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_REELAY_APPS=ON \
      -DBUILD_TESTS=OFF \
    && cmake --build /tmp/reelay/build/ --target install -j$(nproc) \
    && rm -rf /tmp/reelay

FROM ubuntu:22.04 as builder-dejavu

ARG DEJAVU_VERSION=master
ARG DEJAVU_VERSION_TAG=ccff1b6263c738231f101586cbce69a98ed9c59f

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        git \
        curl \
        python3 \
        python3-pip \
        openjdk-17-jre \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/havelund/dejavu.git /tmp/dejavu \
      --branch ${DEJAVU_VERSION} \
    && cd /tmp/dejavu && git checkout ${DEJAVU_VERSION_TAG} \
    && cp /tmp/dejavu/out/dejavu /usr/local/bin/dejavu \
    && cp /tmp/dejavu/out/artifacts/dejavu_jar/dejavu.jar /usr/local/bin/dejavu.jar \
    && chmod +x /usr/local/bin/dejavu

RUN mkdir -p /tmp/dejavu && cd /tmp/dejavu \
    && curl -fL https://github.com/coursier/coursier/releases/latest/download/cs-x86_64-pc-linux.gz | gzip -d > cs \
    && chmod +x cs \
    && ./cs setup -y --dir /usr/local/bin --apps scala:2.11.12,scalac:2.11.12

COPY utils/ /benchmark/utils

RUN cd /tmp/dejavu \
    && python3 -m pip install jsonlines \
    && python3 /benchmark/utils/djv_reelay.py

FROM ocaml/opam:ubuntu-22.04-ocaml-4.11 as builder-monpoly

ARG MONPOLY_VERSION=master

RUN git clone https://bitbucket.org/monpoly/monpoly.git /tmp/monpoly \
      --depth=1 \
      --branch ${MONPOLY_VERSION} \
    && cd /tmp/monpoly \
    && eval $(opam env) \
    && opam install --deps-only . \
    && dune build --release \
    && dune install --prefix /home/opam/.local \
    && rm -rf /tmp/monpoly

FROM ocaml/opam:ubuntu-22.04-ocaml-4.11 as builder-aerial

ARG AERIAL_VERSION=master

RUN sudo apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && sudo apt-get -y install \
        subversion \
        m4 \
        libgmp-dev \
    && sudo apt-get autoremove -y && sudo apt-get clean -y && sudo rm -rf /var/lib/apt/lists/*

RUN opam install ocamlfind menhir safa qcheck

RUN git clone https://bitbucket.org/traytel/aerial.git /tmp/aerial \
      --depth=1 \
      --branch ${AERIAL_VERSION} \
    && cd /tmp/aerial \
    && eval $(opam env) \
    && make \
    && mkdir -p /home/opam/.local/bin/ && cp /tmp/aerial/aerial.native /home/opam/.local/bin/aerial \
    && rm -rf /tmp/aerial

FROM ubuntu:22.04 as builder-timescales

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        git \
        make \
        python3 \
        python3-pip \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/doganulus/timescales.git /tmp/timescales \
      --depth 1 \
    && cd /tmp/timescales \
    && python3 -m pip install -r requirements.txt\
    && make full

COPY utils/ /benchmark/utils

RUN cd /tmp/timescales \
    && python3 /benchmark/utils/tsc_monpoly.py \
    && python3 /benchmark/utils/tsc_dejavu.py

FROM ubuntu:22.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        tini \
        hyperfine \
        python3 \
        python3-pip \
        openjdk-17-jre \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Copy Reelay
COPY --from=builder-reelay /usr/local/bin/ryjson /usr/local/bin/

# Copy Dejavu and examples
COPY --from=builder-dejavu /usr/local/bin/ /usr/local/bin/
COPY --from=builder-dejavu /tmp/dejavu/out/examples /dejavu
COPY --from=builder-dejavu /tmp/dejavu/src/test/scala/tests_atva2020/dejavu-timing-atva-2020 /dejavu/timed

# Copy Monpoly
COPY --from=builder-monpoly /home/opam/.local/ /usr/local/

# Copy Aerial
COPY --from=builder-aerial /home/opam/.local/ /usr/local/

# Copy Timescales benchmarks
COPY --from=builder-timescales /tmp/timescales/fullsuite /timescales/fullsuite

# Copy benchmark scripts
COPY benchmarks/ /benchmarks
COPY resources/ /resources
COPY utils/ /utils

RUN mkdir -p /results

# Copy the script to the container
COPY ./entrypoint.sh /
RUN chmod +x /entrypoint.sh

# Tini correctly initialize the shell in the container
# With the -g option, tini kills the child process group.
# This corresponds more closely to what happens when you do ctrl-C
# See: https://github.com/krallin/tini
ENTRYPOINT ["tini", "-g", "--", "/entrypoint.sh"]

CMD ["/benchmarks/hello.sh"]
