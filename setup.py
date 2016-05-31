from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

setup(
    ext_modules = cythonize([Extension("cpv", ["cython_ext/cpv.pyx"])])
)
