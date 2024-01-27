from distutils.core import setup
from pybind11.setup_helpers import Pybind11Extension
import eigency

ext_modules = [
    Pybind11Extension(
        name="sidereal",
        sources=["src/python_bindings.cpp"],
        include_dirs=['src', *tuple(eigency.get_includes())],
        extra_compile_args=['-std=c++11'],
    ),
]

setup(name = 'sidereal',
    version = '1.0',
    description = 'This package for converting between time systems and coordinate systems',
    ext_modules=ext_modules)