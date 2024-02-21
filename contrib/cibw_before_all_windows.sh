#!/usr/bin/env bash

echo '[build]' > setup.cfg
echo 'compiler = mingw32' >> setup.cfg
echo '[build_ext]' >> setup.cfg
echo 'include_dirs=$(pwd)/install/include' >> setup.cfg
echo 'library_dirs=$(pwd)/install/lib' >> setup.cfg

pacman -S --noconfirm \
    make\
    mingw-w64-x86_64-cmake\
    mingw-w64-x86_64-gcc\
    mingw-w64-x86_64-gmp

python3 -m pip install --user -r contrib/requirements_build.txt

./configure.sh production --auto-download --win64-native --prefix=./install

pushd build
make -j$(( $(nproc) + 1 ))
make install
popd
