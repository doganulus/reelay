FROM debian:12

ENV PIP_BREAK_SYSTEM_PACKAGES 1
ENV CMAKE_GENERATOR=Ninja

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
      sudo \
      git \
      build-essential \
      tar curl wget zip unzip gnupg2 \
      cmake \
      ninja-build \
      python3-pip \
      python-is-python3 \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 

RUN git clone -j$(nproc) --recursive --depth 1 --branch boost-1.82.0 https://github.com/boostorg/boost.git /tmp/boost \
    && cd /tmp/boost \
    && ./bootstrap.sh --prefix=/usr/local --without-libraries=python \
    && ./b2 -j$(nproc) install \
    && cd / && rm -rf /tmp/boost

RUN git clone --depth 1 --branch v3.4.0 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
      -DBUILD_TESTING=OFF \
      -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
      automake \
      autotools-dev \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 

RUN git clone https://github.com/doganulus/cudd.git /tmp/cudd \
    && cd /tmp/cudd \
    && ./configure && aclocal && autoconf \
    && ./configure --enable-silent-rules --enable-shared --enable-obj \
    && make && make install \
    && cd / && rm -rf /tmp/cudd

RUN python -m pip install --upgrade pip && pip install "pybind11[global]"

# Add a container user (good practice)
RUN groupadd reelay -g 1000 \
    && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
    && printf "reelay:reelay" | chpasswd \
    && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

USER reelay

ENV PATH "/home/reelay/.local/bin:${PATH}"

RUN python -m pip install black gcovr 