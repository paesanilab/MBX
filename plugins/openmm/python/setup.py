from setuptools import setup
from setuptools import Extension
import os
import sys
import platform

openmm_dir = '@OPENMM_DIR@'
mbnrgplugin_header_dir = '@MBNRGPLUGIN_HEADER_DIR@'
mbnrgplugin_library_dir = '@MBNRGPLUGIN_LIBRARY_DIR@'

# setup extra compile and link arguments on Mac
extra_compile_args = []
extra_link_args = []

if platform.system() == 'Darwin':
    extra_compile_args += ['-stdlib=libc++', '-mmacosx-version-min=10.7']
    extra_link_args += ['-stdlib=libc++', '-mmacosx-version-min=10.7', '-Wl', '-rpath', openmm_dir+'/lib']

extension = Extension(name='_mbnrgplugin',
                      sources=['MBnrgPluginWrapper.cpp'],
                      libraries=['OpenMM', 'MBnrgPlugin'],
                      include_dirs=[os.path.join(openmm_dir, 'include'), mbnrgplugin_header_dir],
                      library_dirs=[os.path.join(openmm_dir, 'lib'), mbnrgplugin_library_dir],
                      extra_compile_args=extra_compile_args,
                      extra_link_args=extra_link_args
                     )

setup(name='mbnrg',
      version='1.0',
      py_modules=['mbnrgplugin','mbnrg'],
      ext_modules=[extension],
      data_files=[('',['mbnrg.xml'])],
     )
