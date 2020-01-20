import os
import setuptools

from distutils import util
from distutils.core import setup
from distutils.extension import Extension

from distutils.errors import CCompilerError
from distutils.errors import DistutilsExecError
from distutils.errors import DistutilsPlatformError

__version__ = '1.7.0'

def _have_cudd_support():
    import shutil
    import tempfile
    from distutils.ccompiler import new_compiler
    from distutils.sysconfig import customize_compiler

    cpp_code = ('#include "cudd.h"\n#include "cuddObj.hh"\n\n'
              'int main(int argc, char **argv) { return 0; }')
    tmp_dir = tempfile.mkdtemp(prefix='tmp_cudd_')
    bin_file = os.path.join(tmp_dir, 'test_cudd')
    src_file = bin_file + '.cpp'
    with open(src_file, 'w') as fh:
        fh.write(cpp_code)

    compiler = new_compiler()
    customize_compiler(compiler)
    success = False
    try:
        compiler.link_executable(
            compiler.compile([src_file], output_dir=tmp_dir),
            bin_file,
            libraries=['cudd'])
    except CCompilerError:
        print('Unable to compile CUDD - Missing headers?')
    except DistutilsExecError:
        print('Unable to compile CUDD - No C++ compiler?')
    except DistutilsPlatformError:
        print('Unable to compile CUDD - Platform error')
    else:
        success = True
    shutil.rmtree(tmp_dir)
    return success

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
        "include",
        "../include"
        ],
    library_dirs = ['/usr/local/lib'],
    libraries = ['cudd'] if _have_cudd_support() else [],
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
    include_package_data=True,
    zip_safe=False

)