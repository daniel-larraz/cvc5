#!/usr/bin/env bash

if [ "$1" = "true" ]; then
  brew install autoconf

  ./configure.sh production --auto-download --gpl --cln --glpk --cocoa \
    --python-bindings --python-only-src --prefix=./install -DBUILD_GMP=1
else
  ./configure.sh production --auto-download \
    --python-bindings --python-only-src --prefix=./install -DBUILD_GMP=1
fi

SETUP_CFG=./build/src/api/python/setup.cfg
echo "[build_ext]" > ${SETUP_CFG}
echo "include_dirs=$(pwd)/install/include" >> ${SETUP_CFG}
echo "library_dirs=$(pwd)/install/lib" >> ${SETUP_CFG}
cat ${SETUP_CFG}

pushd build
make -j$(( $(sysctl -n hw.logicalcpu) + 1 ))
make install
popd
