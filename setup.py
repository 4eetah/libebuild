from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

setup(
    ext_modules = cythonize([Extension("cpv", ["cython_ext/cpv.pyx", "atom.c", "validate.c"])])
    #ext_modules = cythonize([Extension("atom", ["cython_ext/cpv.pyx"])])
)
