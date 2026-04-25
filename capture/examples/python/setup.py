from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

ext_modules = [
    Pybind11Extension(
        "auralis_capture",
        ["binding.cpp"],
        cxx_std=17,
        libraries=["auralis_capture"],
    ),
]

setup(
    name="auralis_capture",
    version="0.2.0",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
