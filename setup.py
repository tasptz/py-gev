from skbuild import setup
from pathlib import Path

PYBIND11_SRC = str(Path(__file__).absolute().parent / 'ext' / 'pybind11')

setup(
    name='gev',
    version='1.0.0',
    packages=['gev'],
    cmake_args=[f'-DPYBIND11_SRC={PYBIND11_SRC}']
)
