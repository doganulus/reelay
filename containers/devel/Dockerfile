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
