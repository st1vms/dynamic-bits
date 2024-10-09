# distutils: sources = pydbits/src/*.c
# distutils: include_dirs = pydbits/include/

from setuptools import Extension, setup
from Cython.Build import cythonize

setup(
    name="pydbits",
    version="0.1.5",
    ext_modules=cythonize(
        [Extension("pydbits", ["pydbits.pyx"], libraries=["dynamic-bits"])],

        language_level="3",
    ),
)
