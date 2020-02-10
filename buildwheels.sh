#!/bin/bash 
# This script is to build manylinux2014 wheels for PyPi in Docker 
#
# Older manylinux1 and manylinux2010 standard environments do not provide
# Boost Interval Container Library (ICL), which is introduced in Boost v1.46.
#
# docker pull quay.io/pypa/manylinux2014_x86_64:latest
# docker run -it quay.io/pypa/manylinux2014_x86_64:latest

# Install dependencies and source code
yum install -y boost-devel

git clone https://github.com/doganulus/reelay --recurse-submodules

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install pybind11
    "${PYBIN}/pip" wheel /reelay/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" --plat manylinux2014_x86_64 -w /reelay/wheelhouse/
done