FROM ubuntu:24.04 as ryjson-prebuilt

RUN git clone https://github.com/doganulus/reelay.git /tmp/reelay \
      --depth 1 \
      --branch main \
    && cmake -S/tmp/reelay -B/tmp/reelay/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_REELAY_APPS=ON \
      -DBUILD_TESTS=OFF \
    && cmake --build /tmp/reelay/build/ --target install -j$(nproc) \
    && rm -rf /tmp/reelay

FROM ubuntu:24.04 as ryjson
ARG TARGETARCH TARGETOS TARGETPLATFORM TARGETVARIANT

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        tini \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

COPY --from=ryjson-prebuilt /usr/local/bin/ryjson /usr/local/bin/

COPY ./ryjson/entrypoint.sh /
RUN chmod +x /entrypoint.sh

# Tini correctly initialize the shell in the container
# With the -g option, tini kills the child process group.
# This corresponds more closely to what happens when you do ctrl-C
# See: https://github.com/krallin/tini
ENTRYPOINT ["tini", "-g", "--", "/entrypoint.sh"]
WORKDIR /app
CMD ["--help"]
