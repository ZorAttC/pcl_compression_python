import os
import subprocess
import sys
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = [
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}',
            f'-DPYTHON_EXECUTABLE={sys.executable}'
        ]
        build_args = ['--config', 'Release', '--', '-j4']

        env = os.environ.copy()
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)


setup(
    name='pcl_compression',#the package name you use to install 
    version='0.0.1',
    author='Zoratt',
    author_email='1720049199@qq.com',
    description='A pcl octree compression algorithm pybind example project',
    ext_modules=[CMakeExtension('_pcl_compression')],#the module name you use to import
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    packages=['pcl_compression'],
)
