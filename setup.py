import os
import sys
import platform
import subprocess
import setuptools

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

__version__ = '2005.0'

cxxflags = [
    '--std=c++11',          # C++11 required for manylinux2014
    '-fvisibility=hidden',  # Produces smaller shared objects (see pybind11)
    ]

if platform.system() == 'Linux':
    cxx = os.environ.get('CXX', 'g++')
    if cxx.startswith('g++'):
        cxxflags += ['-fno-new-ttp-matching']
    if cxx.startswith('clang++'):
        cxxflags += ['-fsized-deallocation']
elif platform.system() == 'Darwin':
    cxx = os.environ.get('CXX', 'clang++')
    if cxx.startswith('g++'):
        cxxflags += ['-fno-new-ttp-matching']
    if cxx.startswith('clang++'):
        cxxflags += ['-fsized-deallocation']
elif platform.system() == 'Windows':
    raise RuntimeError("Windows builds are not supported.")
else:
    raise RuntimeError("Unknown platform. Build did not complete.")


class BuildWithCudd(build_ext):
    def run(self):
        self.build_cudd()
        super(BuildWithCudd, self).run()

    def build_cudd(self):
        cudd_dir = os.path.abspath('third_party/cudd')
        subprocess.check_call(
            ['./configure', '--enable-obj', '--enable-silent-rules'],
            cwd=cudd_dir)
        subprocess.check_call(['make'], cwd=cudd_dir)


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


ext_cpp_library = Extension(
    'library',
    sources=['python/src/main.cpp'],
    include_dirs=[
        "include",
        'python/include',
        'third_party/cudd/cudd',
        'third_party/cudd/cplusplus',
        get_pybind_include(),
        get_pybind_include(user=True),
    ],
    # library_dirs=['usr/local/lib'],
    # libraries=['cudd'],
    extra_compile_args=cxxflags,
    extra_objects=[
        'third_party/cudd/cudd/.libs/libcudd.a'],
    language='c++'
)

setup(
    name='reelay',
    version=__version__,
    author='Dogan Ulus',
    author_email='doganulus@gmail.com',
    url='http://github.com/doganulus/reelay',
    package_dir={'': 'python'},
    packages=setuptools.find_packages(where='python'),
    license='MPL-2.0',
    description='Runtime verification package using formal specifications',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Mathematics',
        'Topic :: Software Development :: Testing',
        'License :: OSI Approved :: Mozilla Public License 2.0 (MPL 2.0)',
        'Programming Language :: Python :: 3',
    ],
    python_requires='>=3',
    install_requires=['pybind11>=2.4'],
    setup_requires=['pybind11>=2.4'],
    tests_require=['pytest'],
    ext_package='reelay',
    ext_modules=[ext_cpp_library],
    cmdclass=dict(build_ext=BuildWithCudd),
    include_package_data=True,
    zip_safe=False
)
