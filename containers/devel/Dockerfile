ARG REELAY_VERSION=main

ARG REELAY_IMAGE_REGISTRY=localhost
ARG REELAY_IMAGE_REGISTRY_REMOTE=${REELAY_IMAGE_REGISTRY}

FROM docker.io/library/ubuntu:24.04 AS reelay-builder
ARG TARGETARCH TARGETOS TARGETPLATFORM TARGETVARIANT

ARG REELAY_VERSION
ENV REELAY_VERSION=${REELAY_VERSION}

RUN apt-get update && \
    apt-get install -y \
      curl \
      wget \
      unzip \
      gnupg2 \
      git \
      make \
      cmake \
      ninja-build \
      diffutils \
      patch \
      hyperfine \
      && \
    apt-get clean && \
  rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    apt-get install -y \
      clang \
      clangd \
      clang-tidy \
      clang-format \
      clang-tools \
      catch2 \
      lcov \
      valgrind \
      && \
    apt-get clean && \
  rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    apt-get install -y \
      python3 \
      python3-pip \
      python3-setuptools \
      python3-dev \
      python-is-python3 \
      && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN pip3 install --no-cache --break-system-packages \
      cmake-format \
      pytest

FROM docker.io/library/ubuntu:24.04 AS reelay-devel
ARG TARGETARCH TARGETOS TARGETPLATFORM TARGETVARIANT
