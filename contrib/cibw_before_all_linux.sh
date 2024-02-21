#!/usr/bin/env bash

python3 -m pip install --user -r contrib/requirements_build.txt

./configure.sh production --auto-download --prefix=./install

pushd build
make -j$(( $(nproc) + 1 ))
make install
popd
