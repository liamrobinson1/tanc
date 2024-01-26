from distutils.core import setup
import numpy
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "tanc",
        ["src/python_bindings.cpp"],
        include_dirs=['src', numpy.get_include()],
    ),
]

setup(name = 'tanc',
    version = '1.0',
    description = 'This is a demo package',
    ext_modules=ext_modules)