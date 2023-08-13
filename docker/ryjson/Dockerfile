#
# docker build -t ghcr.io/doganulus/reelay:ryjson docker/ryjson
#
FROM ghcr.io/doganulus/reelay-devel:latest as builder

RUN git clone https://github.com/doganulus/reelay.git /tmp/reelay \
      --depth 1 \
      --branch main \
    && cd /tmp/reelay \
    && cmake -S/tmp/reelay -B/tmp/reelay/build \
      -DBUILD_REELAY_APPS=ON \
      -DBUILD_TESTS=OFF \
    && cmake --build /tmp/reelay/build/ --target install -j$(nproc) \
    && cd / && rm -rf /tmp/reelay

#
# docker run --rm -v $(pwd):/app ghcr.io/doganulus/reelay:ryjson
#
FROM debian:12

COPY --from=builder /usr/local/bin/ryjson /usr/local/bin/

WORKDIR /app

CMD [ "ryjson" ]
