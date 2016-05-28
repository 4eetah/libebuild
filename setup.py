from distutils.core import setup, Extension
#modules = Extension('boolean', sources = ['boolean_py.c', 'boolean.c'])
modules = Extension('cpv', sources = ['cpv_py.c', 'cpv.c'])
setup(name = 'libebuild',
      version = '1.0',
      description = 'libebuild',
      ext_modules = [modules]
      )
