#!/bin/bash 
# This script is to build manylinux2014 wheels for PyPi in Docker 
#
# Older manylinux1 and manylinux2010 standard environments do not provide
# Boost Interval Container Library (ICL), which is introduced in Boost v1.46.
#
# docker pull quay.io/pypa/manylinux2014_x86_64:latest
# docker run -v $(pwd):/monitors -e HOST_UID=$(id -u) -it quay.io/pypa/manylinux2014_x86_64:latest /monitors/buildwheels.sh

# Install dependencies and source code
yum install -y boost-devel

# Clone reelay
git clone https://github.com/doganulus/reelay --recurse-submodules

# Prepare source distribution
cd /reelay
/opt/python/cp38-cp38/bin/python3 setup.py sdist --dist-dir=/monitors/release/
cd /

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install pybind11
    "${PYBIN}/pip" wheel /reelay/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" --plat manylinux2014_x86_64 -w /monitors/release
done

chown -R ${HOST_UID}:${HOST_UID} /monitors/release

# python3 -m pip install twine
# python3 -m twine upload --repository testpypi release/*
# python3 -m twine upload release/*
