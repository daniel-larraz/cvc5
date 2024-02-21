#!/usr/bin/env bash

echo "[build]" > setup.cfg
echo "compiler = mingw32" >> setup.cfg
echo "[build_ext]" >> setup.cfg
echo "include_dirs=$(cygpath -m $(pwd)/install/include)" >> setup.cfg
echo "library_dirs=$(cygpath -m $(pwd)/install/lib);C:\Windows\SysWOW64" >> setup.cfg
cat setup.cfg

python3 -m pip install --user -r contrib/requirements_build.txt

./configure.sh production --auto-download --win64-native --prefix=./install

pushd build
make -j$(( $(nproc) + 1 ))
make install
popd
