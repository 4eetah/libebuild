from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

libebuild_modules = [
    Extension("cpv", ["cython_ext/cpv.pyx", "validate.c", "version.c"]),
    Extension("atom", ["cython_ext/atom.pyx", "validate.c", "version.c"]),
]

setup(
    name = "libebuild",
    ext_modules = cythonize(libebuild_modules),
)
