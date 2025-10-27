#
# docker build -t ghcr.io/doganulus/reelay:ryjson docker/ryjson
#
FROM ghcr.io/doganulus/reelay-devel:latest as builder

RUN git clone https://github.com/doganulus/reelay.git /tmp/reelay \
      --depth 1 \
      --branch main \
    && cmake -S/tmp/reelay -B/tmp/reelay/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_REELAY_APPS=ON \
      -DBUILD_TESTS=OFF \
    && cmake --build /tmp/reelay/build/ --target install -j$(nproc) \
    && rm -rf /tmp/reelay

#
# docker run --rm -v $(pwd):/app ghcr.io/doganulus/reelay:ryjson
#
FROM debian:12

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        tini \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Copy the script to the container
COPY ./entrypoint.sh /
RUN chmod +x /entrypoint.sh

# Tini correctly initialize the shell in the container
# With the -g option, tini kills the child process group.
# This corresponds more closely to what happens when you do ctrl-C
# See: https://github.com/krallin/tini

COPY --from=builder /usr/local/bin/ryjson /usr/local/bin/

WORKDIR /app

ENTRYPOINT ["tini", "-g", "--", "/entrypoint.sh"]
CMD ["--help"]
