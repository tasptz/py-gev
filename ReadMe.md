# Python GEV Wrapper
This package provides a lightweight python interface to the [Linux GigE-V Framework](https://www.teledynedalsa.com/en/products/imaging/vision-software/linux-gige-v-framework/).

## Installation
First
```
pip install -r requirements.txt
git submodule update --init --recursive
```
then
```
python setup.py bdist_wheel
pip install dist/gev-*.whl
```
or just
```
python setup.py develop
```
For the example you need the `Pillow` module, then run `python examples/write_frames.py`.
