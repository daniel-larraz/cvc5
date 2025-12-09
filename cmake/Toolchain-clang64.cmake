###############################################################################
# Top contributors (to current version):
#   Daniel Larraz
#
# This file is part of the cvc5 project.
#
# Copyright (c) 2009-2025 by the authors listed in the file AUTHORS
# in the top-level source directory and their institutional affiliations.
# All rights reserved.  See the file COPYING in the top-level source
# directory for licensing information.
# #############################################################################
#
# Toolchain file for building for Windows from Ubuntu.
#
# Use: cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-clang64.cmake
##

SET(CMAKE_SYSTEM_NAME Windows)


set(LLVM_SYSROOT "${CMAKE_SOURCE_DIR}/deps/llvm")

# Set target environment path
SET(CMAKE_FIND_ROOT_PATH "${LLVM_SYSROOT}")

SET(CMAKE_C_COMPILER ${LLVM_SYSROOT}/bin/clang.exe)
SET(CMAKE_CXX_COMPILER ${LLVM_SYSROOT}/bin/clang++.exe)

# Adjust the default behaviour of the find_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXE_LINKER_FLAGS "-L${LLVM_SYSROOT}/lib")