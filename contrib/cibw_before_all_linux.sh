#!/usr/bin/env bash

python3 -m pip install --user -r contrib/requirements_build.txt

./configure.sh production --auto-download --python-bindings --only-python-ext-src --prefix=./install

cd build/src/src/api/python/
