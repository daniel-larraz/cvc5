#!/usr/bin/env python
###############################################################################
# Top contributors (to current version):
#   Daniel Larraz
#
# This file is part of the cvc5 project.
#
# Copyright (c) 2009-2024 by the authors listed in the file AUTHORS
# in the top-level source directory and their institutional affiliations.
# All rights reserved.  See the file COPYING in the top-level source
# directory for licensing information.
# #############################################################################

import os
from setuptools import setup

ext_suffix = 'pyd' if os.name == 'nt' else 'so'
ext_filename = 'cvc5_python_base.' + ext_suffix

packages=['cvc5', 'cvc5.pythonic']
license_files=["COPYING", "licenses/*"]

# If we have already compiled the Python extension module,
# include the module directly instead of recompiling it.
if os.path.isfile(os.path.join('cvc5', ext_filename)):
    setup(
        packages=packages,
        package_data={'': [ext_filename]},
        # Let setuptools know the package include an extension module
        # so that the generated wheel name includes
        # the python version, the ABI tag, and the platform name.
        has_ext_modules=lambda: True,
        license_files=license_files
    )
else:
    import sys
    import sysconfig
    from setuptools.extension import Extension

    from Cython.Distutils import build_ext
    from Cython.Build import cythonize

    if sys.platform == 'darwin':
        macos_ver = "11.0" if platform.machine() == "arm64" else "10.13"
        os.environ.setdefault("MACOSX_DEPLOYMENT_TARGET", macos_ver)

    compiler_directives = {
        'language_level': 3,
        'binding': False,
    }

    if sys.platform == 'win32':
        os.environ['PATH'] += r';C:\msys64\mingw64\bin'

    ext_options = {
        "libraries" : ["cvc5", "cvc5parser"],
        "language" : "c++",
        "extra_compile_args" : ["-std=c++17", "-fno-var-tracking"]
    }

    mod_name = "cvc5.cvc5_python_base"
    mod_src_files = ["cvc5_python_base.pyx"]

    ext_module = Extension(mod_name, mod_src_files, **ext_options)
    ext_module.cython_directives = {"embedsignature": True}

    class ExtensionBuilder(build_ext):
        def get_ext_filename(self, ext_name):
            # Do not include the python version, the ABI tag, and
            # the platform name in the extension module name.
            # This facilitates specifying the dependencies of
            # the module in CMake.
            filename = super().get_ext_filename(ext_name)
            suffix = sysconfig.get_config_var('EXT_SUFFIX')
            ext = os.path.splitext(filename)[1]
            return filename.replace(suffix, "") + ext

    setup(
        cmdclass={'build_ext': ExtensionBuilder},
        ext_modules=cythonize([ext_module], compiler_directives=compiler_directives),
        packages=packages,
        license_files=license_files
    )
