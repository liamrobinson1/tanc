from distutils.core import setup
import numpy
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        name="tanc",
        sources=["src/python_bindings.cpp"],
        include_dirs=['src', numpy.get_include(), '/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3'],
    ),
]

setup(name = 'tanc',
    version = '1.0',
    description = 'This package for converting between time systems and coordinate systems',
    ext_modules=ext_modules)