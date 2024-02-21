#!/usr/bin/env bash

python3 -m pip install --user -r contrib/requirements_build.txt

./configure.sh production --auto-download

pushd build
make -j$(( $(nproc) + 1 ))
popd

export CPLUS_INCLUDE_PATH=./include:./build/include
export LIBRARY_PATH=./build/src:./build/src/parser