from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

libebuild_modules = [
    Extension("cpv", ["cython_ext/cpv.pyx", "atom.c", "validate.c"]),
    Extension("atom", ["cython_ext/atom.pyx", "validate.c"]),
]

setup(
    name = "libebuild",
    ext_modules = cythonize(libebuild_modules),
)
