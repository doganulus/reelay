import os
import setuptools

from distutils import util
from distutils.core import setup
from distutils.extension import Extension

__version__ = '1.6.0'

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
    sources=['src/main.cpp'],
    include_dirs=[
        # Path to pybind11 headers
        get_pybind_include(),
        get_pybind_include(user=True),
        "/usr/local/include"
    ],
    extra_compile_args = ["-O2", "--std=c++17", "-fPIC", "-pthread", "-fno-new-ttp-matching"],
    language='c++'
)

setup(
    name='reelay',
    version=__version__,
    author='Dogan Ulus',
    author_email='doganulus@gmail.com',
    url='http://github.com/doganulus/reelay',
    packages=setuptools.find_packages(),
    license='MPL-2.0',
    description = 'Runtime verification package using formal specifications',
    python_requires='>=3.6',
    install_requires=['pybind11>=2.4'],
    setup_requires=['pybind11>=2.4'],
    ext_package='reelay',
    ext_modules=[ext_recipes],
    zip_safe=False
)