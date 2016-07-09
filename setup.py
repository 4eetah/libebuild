#!/usr/bin/env python

from distutils.command import install_lib as dst_install_lib
from distutils.core import setup, Command
from setuptools import find_packages
from distutils.extension import Extension
from Cython.Build import cythonize

libebuild_modules = [
    Extension("libebuild.cpv",
             ["libebuild/cpv.pyx", "cpv.c", "validate.c", "version.c", "error.c"]),
    Extension("libebuild.atom",
             ["libebuild/atom.pyx", "atom.c", "validate.c", "version.c", "error.c"]),
    Extension("libebuild.version",
             ["libebuild/version.pyx", "version.c", "validate.c", "error.c"]),
]

setup(
    name = "libebuild",
    version = "0.1.0",
    license = "BSD-2",
    ext_modules = cythonize(libebuild_modules),
    packages = find_packages(),
)
