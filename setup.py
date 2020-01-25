import os
import re
import sys
import platform
import subprocess

import setuptools

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

__version__ = '2001'


class ReelayRecipesExtension(Extension):
    def __init__(self, name, sourcedir='', cudd_src_dir='cudd'):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.include_dirs = os.path.abspath(sourcedir)


class BuildWithCudd(build_ext):
    def run(self):

        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        # if platform.system() == "Windows":
        #     raise RuntimeError("Currently Windows is not supported")

        self.build_cudd_with_cmake()
        super(BuildWithCudd, self).run()

    def build_cudd_with_cmake(self, cudd_src_dir='third_party/cudd'):
        cudd_dir = os.path.abspath(cudd_src_dir)
        build_dir = os.path.abspath('build/cudd_build')
        if not os.path.exists(build_dir):
            os.makedirs(build_dir)

        # This is a hack for WSL due to dummy permission errors that
        # send an error signal while everything configured correctly
        try:
            subprocess.check_call(['cmake', cudd_dir], cwd=build_dir)
        except subprocess.CalledProcessError:
            subprocess.check_call(['cmake', cudd_dir], cwd=build_dir)

        subprocess.check_call(['make'], cwd=build_dir)


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


ext_recipes = Extension(
    'recipes',
    sources=['python/src/main.cpp'],
    include_dirs=[
        # Path to pybind11 headers
        get_pybind_include(),
        get_pybind_include(user=True),
        "include",
    ],
    # library_dirs = ['/usr/local/lib'],
    # libraries = ['cudd'] if _have_cudd_support() else [],
    extra_compile_args=["-O3", "--std=c++17",
                        "-fPIC", "-fno-new-ttp-matching"],
    extra_objects=['build/cudd_build/cplusplus/libcuddObj.a'],
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
    ext_package='reelay',
    ext_modules=[ext_recipes],
    cmdclass=dict(build_ext=BuildWithCudd),
    include_package_data=True,
    zip_safe=False

)
