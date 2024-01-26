from distutils.core import setup, Extension
import numpy

tanc_module = Extension('tanc',
                    sources = ['cpp/python_bindings.cpp'],
                    include_dirs = ['cpp', numpy.get_include()],
                    extra_compile_args = ['-std=c++11'])

setup(name = 'tanc',
        version = '1.0',
        description = 'This is a demo package',
        ext_modules = [tanc_module])

