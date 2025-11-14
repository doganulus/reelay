FROM docker.io/library/almalinux:8
ARG TARGETARCH TARGETOS TARGETPLATFORM TARGETVARIANT

RUN dnf install -y \
      dnf-plugins-core \
      dnf-plugin-config-manager \
      epel-release \
      && \
    dnf config-manager --enable powertools && \
    dnf install -y \
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
      lcov \
      valgrind \
      && \
    dnf clean all

RUN dnf install -y \
      python3.12 \
      python3.12-pip \
      python3.12-setuptools \
      python3.12-devel \
      && \
    dnf clean all

RUN git clone --depth 1 --branch v3.11.0 https://github.com/catchorg/Catch2.git /tmp/catch2 && \
    cmake -S/tmp/catch2 -B/tmp/catch2/build \
      -DBUILD_TESTING=OFF \
      -DCATCH_INSTALL_DOCS=OFF \
      && \
    cmake --build /tmp/catch2/build/ --target install -j$(nproc) && \
    rm -rf /tmp/catch2

RUN pip3 install --no-cache --upgrade pip setuptools && \
    pip3 install --no-cache \
      cmake-format \
      pytest

