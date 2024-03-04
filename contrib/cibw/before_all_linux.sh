#!/usr/bin/env bash

python3 -m pip install --user -r contrib/requirements_build.txt
python3 -m pip install --user -r contrib/requirements_python_dev.txt

./configure.sh production --auto-download --python-bindings --prefix=./install

pushd build
make -j$(( $(nproc) + 1 )) cvc5_python_cibw
make install
popd
