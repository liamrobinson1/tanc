from distutils.core import setup
from pybind11.setup_helpers import Pybind11Extension
import eigency
import platform

std_arg = '-std=c++17'
if platform.system() == 'Windows':
    std_arg = '/std:c++17'

ext_modules = [
    Pybind11Extension(
        name="sidereal",
        sources=["src/python_bindings.cpp"],
        include_dirs=['src', *tuple(eigency.get_includes())],
        extra_compile_args=[std_arg],
    ),
]

setup(name = 'sidereal',
    ext_modules=ext_modules)