#!/bin/sh

python3 ../setup.py build_ext -if &&
  python3 pywireshark.py