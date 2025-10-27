FROM quay.io/pypa/manylinux_2_28:latest
ARG TARGETARCH TARGETOS TARGETPLATFORM TARGETVARIANT

RUN dnf install -y \
      curl \
      wget \
      unzip \
      git \
      make \
      cmake \
      ninja-build \
      diffutils \
      patch \
      clang \
      clangd \
      clang-analyzer \
      clang-tools-extra \
      && \
    dnf clean all

RUN --mount=type=bind,source=./scripts/boost-source-cmake-install.sh,target=/container-build/scripts/boost-source-cmake-install.sh \
    . /container-build/scripts/boost-source-cmake-install.sh

RUN mkdir -p /tmp/boost &&\
    curl -L https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz | tar -xz --strip-components=1 -C /tmp/boost && \
    cmake -S /tmp/boost -B /tmp/boost/build \
      -DBOOST_ENABLE_MPI=OFF \
      -DBOOST_ENABLE_PYTHON=OFF \
      -DBUILD_SHARED_LIBS=OFF \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      && \
    cmake --build /tmp/boost/build --target install -- -j"$(nproc)" && \
    rm -rf /tmp/boost

RUN git clone --depth 1 --branch v3.11.0 https://github.com/catchorg/Catch2.git /tmp/catch2 && \
    cmake -S/tmp/catch2 -B/tmp/catch2/build \
      -DBUILD_TESTING=OFF \
      -DCATCH_INSTALL_DOCS=OFF \
      && \
    cmake --build /tmp/catch2/build/ --target install -j$(nproc) && \
    rm -rf /tmp/catch2

RUN git clone --depth 1 --branch v4.0.7 https://github.com/lemire/simdjson.git /tmp/simdjson && \
    cmake -S/tmp/simdjson -B/tmp/simdjson/build && \
    cmake --build /tmp/simdjson/build/ --target install -j$(nproc) && \
    rm -rf /tmp/simdjson
