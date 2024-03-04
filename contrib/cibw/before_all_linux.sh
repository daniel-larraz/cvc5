#!/usr/bin/env bash

python3 -m pip install --user -r contrib/requirements_build.txt
python3 -m pip install --user -r contrib/requirements_python_dev.txt

./configure.sh production --auto-download --python-bindings --prefix=./install

SETUP_CFG = ./build/src/api/python/setup.cfg
echo "[build_ext]" > ${SETUP_CFG}
echo "include_dirs=$(pwd)/install/include" >> ${SETUP_CFG}
echo "library_dirs=$(pwd)/install/lib64" >> ${SETUP_CFG}
cat ${SETUP_CFG}

pushd build
make -j$(( $(nproc) + 1 )) cvc5_python_cibw
make install
popd
