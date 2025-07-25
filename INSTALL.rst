Building cvc5
-------------

.. code:: bash

    ./configure.sh
        # use --prefix to specify an install prefix (default: /usr/local)
        # use --name=<PATH> for custom build directory
        # use --auto-download to download and build missing, required or
        #   enabled, dependencies
    cd <build_dir>   # default is ./build
    make             # use -jN for parallel build with N threads
    make check       # to run default set of tests
    make install     # to install into the prefix specified above

All binaries are built into ``<build_dir>/bin``, the cvc5 libraries are built into
``<build_dir>/lib``.


Supported Operating Systems
---------------------------

cvc5 can be built natively on Linux and macOS. Native compilation on Windows is also
possible using MSYS2. Additionally, cvc5 supports cross-compilation for x86_64 Windows
using Mingw-w64 and for ARM64 on both Linux and macOS.
We generally recommend a 64-bit operating system.


Compilation on macOS
^^^^^^^^^^^^^^^^^^^^

On macOS, we recommend using `Homebrew <https://brew.sh/>`_ to install the
dependencies.  We also have a Homebrew Tap available at
https://github.com/cvc5/homebrew-cvc5.
Note that linking system libraries statically is
`strongly discouraged <https://developer.apple.com/library/archive/qa/qa1118/_index.html>`_
on macOS. Using ``./configure.sh --static`` will thus produce a binary
that uses static versions of all our dependencies, but is still a dynamically
linked binary.


Compilation on Windows
^^^^^^^^^^^^^^^^^^^^^^

Install `MSYS2 <https://www.msys2.org/>`_ and `Python <https://www.python.org/downloads/windows/>`_ on your system.
Launch a `MINGW64 environment <https://www.msys2.org/docs/environments/>`_ and
install the required packages for building cvc5:

.. code:: bash

  pacman -S git make mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-gmp zip

Clone the cvc5 repository and follow the general build steps above.
The built binary ``cvc5.exe`` and the DLL libraries are located in
``<build_dir>/bin``. The import libraries and the static libraries
can be found in ``<build_dir>/lib``.


Cross-compiling for Windows
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Cross-compiling cvc5 for Windows requires the POSIX version of MinGW-w64.
On some Linux distributions, this is the default variant. On others, like Ubuntu,
you may need to set it manually as follows:

.. code:: bash

  sudo update-alternatives --set x86_64-w64-mingw32-gcc /usr/bin/x86_64-w64-mingw32-gcc-posix
  sudo update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix

Once the POSIX variant of MinGW-w64 is installed and set on your system,
you can cross-compile cvc5 as follows:

.. code:: bash

  ./configure.sh --win64 --static <configure options...>

  cd <build_dir>   # default is ./build
  make             # use -jN for parallel build with N threads

The built binary ``cvc5.exe`` and the DLL libraries are located in
``<build_dir>/bin``. The import libraries and the static libraries
can be found in ``<build_dir>/lib``.


WebAssembly Compilation
^^^^^^^^^^^^^^^^^^^^^^^^

Compiling cvc5 to WebAssembly needs the Emscripten SDK (version 3.1.18 or 
latter). Setting up emsdk can be done as follows:

.. code:: bash

  git clone https://github.com/emscripten-core/emsdk.git
  cd emsdk
  ./emsdk install <version>   # <version> = '3.1.18' is preferable, but 
                              # <version> = 'latest' has high chance of working
  ./emsdk activate <version>
  source ./emsdk_env.sh   # Activate PATH and other environment variables in the
                          # current terminal. Whenever Emscripten is going to be
                          # used this command needs to be called before because 
                          # emsdk doesn't insert the binaries paths directly in 
                          # the system PATH variable.

Refer to the `emscripten dependencies list <https://emscripten.org/docs/getting_started/downloads.html#platform-specific-notes>`_ 
to ensure that all required dependencies are installed on the system.

Then, in the cvc5 directory:

.. code:: bash

  ./configure.sh --static --static-binary --auto-download --wasm=<value> --wasm-flags='<emscripten flags>' <configure options...>

  cd <build_dir>   # default is ./build
  make             # use -jN for parallel build with N threads

``--wasm`` can take three values: ``WASM`` (will generate the wasm file for cvc5), ``JS``
(not only the wasm, but the .js glue code for web integration) and ``HTML`` (both
the last two files and also an .html file which supports the run of the glue
code).

``--wasm-flags`` take a string wrapped by a single quote containing the
`emscripten flags <https://github.com/emscripten-core/emscripten/blob/main/src/settings.js>`_,
which modifies how the wasm and glue code are built and how they behave. An ``-s``
should precede each flag.

For example, to generate a HTML page, use:

.. code:: bash

  ./configure.sh --static --static-binary --auto-download --wasm=HTML --name=prod

  cd prod
  make            # use -jN for parallel build with N threads

After that, you can run ``python -m http.server`` within ``prod/bin``, open http://0.0.0.0:8000/cvc5.html with Chrome to visualize the page generated by Emscripten, write down a valid SMTLIB input, and press ESC twice to obtain its output.

On the other hand, to generate a modularized glue code to be imported by custom web pages, use:

.. code:: bash

  ./configure.sh --static --static-binary --auto-download --wasm=JS --wasm-flags='-s MODULARIZE' --name=prod

  cd prod
  make            # use -jN for parallel build with N threads

Build dependencies
------------------

cvc5 makes uses of a number of tools and libraries. Some of these are required
while others are only used with certain configuration options. If
``--auto-download`` is given, cvc5 can automatically download and build most
libraries that are not already installed on your system. If your libraries are
installed in a non-standard location, you can use ``--dep-path`` to define an
additional search path for all dependencies. Versions given are minimum
versions; more recent versions should be compatible.

- `GNU C and C++ (gcc and g++, >= 7) <https://gcc.gnu.org>`_
  or `Clang (>= 5) <https://clang.llvm.org>`_
- `CMake >= 3.16 <https://cmake.org>`_
- `GNU Make <https://www.gnu.org/software/make/>`_
  or `Ninja <https://ninja-build.org/>`_
- `Python >= 3.7 <https://www.python.org>`_
  + module `tomli <https://pypi.org/project/tomli/>`_ (Python < 3.11)
  + module `pyparsing <https://pypi.org/project/pyparsing/>`_
- `GMP v6.3 (GNU Multi-Precision arithmetic library) <https://gmplib.org>`_
- `CaDiCaL >= 2.1.0 (SAT solver) <https://github.com/arminbiere/cadical>`_
- `SymFPU <https://github.com/martin-cs/symfpu/tree/CVC4>`_

If ``--auto-download`` is given, the Python modules will be installed automatically in
a virtual environment if they are missing. To install the modules globally and skip
the creation of the virtual environment, configure cvc5 with ``./configure.sh --no-pyvenv``.

CaDiCaL (SAT solver)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`CaDiCaL <https://github.com/arminbiere/cadical>`_ is a SAT solver that can be
used for the bit-vector solver. It can be downloaded and built automatically.


GMP (GNU Multi-Precision arithmetic library)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

GMP is usually available on your distribution and should be used from there. If
it is not, or you want to cross-compile, or you want to build cvc5 statically
but the distribution does not ship static libraries, cvc5 builds GMP
automatically when ``--auto-download`` is given.


SymFPU (Support for the Theory of Floating Point Numbers)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`SymFPU <https://github.com/martin-cs/symfpu/tree/CVC4>`_ is an implementation
of SMT-LIB/IEEE-754 floating-point operations in terms of bit-vector operations.
It is required for supporting the theory of floating-point numbers and can be
downloaded and built automatically.


Optional Dependencies
---------------------


CryptoMiniSat (Optional SAT solver)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`CryptoMinisat <https://github.com/msoos/cryptominisat>`_ is a SAT solver that
can be used for solving bit-vector problems with eager bit-blasting. This
dependency may improve performance. It can be downloaded and built
automatically. Configure cvc5 with ``configure.sh --cryptominisat`` to build
with this dependency. Minimum version required is ``5.11.2``.


Kissat (Optional SAT solver)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`Kissat <https://github.com/arminbiere/kissat>`_ is a SAT solver that can be
used for solving bit-vector problems with eager bit-blasting. This dependency
may improve performance. It can be downloaded and built automatically. Configure
cvc5 with ``configure.sh --kissat`` to build with this dependency.


LibPoly >= v0.2.0 (Optional polynomial library)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`LibPoly <https://github.com/SRI-CSL/libpoly>`_ is required for CAD-based
nonlinear reasoning. It can be downloaded and built automatically. Configure
cvc5 with ``configure.sh --poly`` to build with this dependency.

CoCoA (Optional computer algebra library)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`CoCoA <https://cocoa.dima.unige.it/cocoa/>`_ is required for some non-linear
reasoning and for finite field reasoning. We use a patched version of it, so we
recommend downloading it using the ``--auto-download`` configuration flag,
which applies our patch automatically. It is included in the build through the
``--cocoa --gpl`` configuration flag.

CoCoA is covered by the GPLv3 license. See below for the ramifications of this.

CLN >= v1.3 (Class Library for Numbers)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`CLN <http://www.ginac.de/CLN>`_ is an alternative multiprecision arithmetic
package that may offer better performance and memory footprint than GMP.
Configure cvc5 with ``configure.sh --cln --gpl`` to build with this dependency.

Note that CLN is covered by the `GNU General Public License, version 3
<https://www.gnu.org/licenses/gpl-3.0.en.html>`_. If you choose to use cvc5 with
CLN support, you are licensing cvc5 under that same license. (Usually cvc5's
license is more permissive than GPL, see the file `COPYING` in the cvc5 source
distribution for details.)


glpk-cut-log (A fork of the GNU Linear Programming Kit)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`glpk-cut-log <https://github.com/timothy-king/glpk-cut-log/>`_ is a fork of
`GLPK <http://www.gnu.org/software/glpk/>`_ (the GNU Linear Programming Kit).
This can be used to speed up certain classes of problems for the arithmetic
implementation in cvc5. (This is not recommended for most users.)

cvc5 is not compatible with the official version of the GLPK library.
To use the patched version of it, we recommend downloading it using
the ``--auto-download`` configuration flag, which applies
the patch automatically.
Configure cvc5 with ``configure.sh --glpk --gpl`` to build with this dependency.

Note that GLPK and glpk-cut-log are covered by the `GNU General Public License,
version 3 <https://www.gnu.org/licenses/gpl-3.0.en.html>`_. If you choose to use
cvc5 with GLPK support, you are licensing cvc5 under that same license. (Usually
cvc5's license is more permissive; see above discussion.)


Editline library (Improved Interactive Experience)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The `Editline Library <https://thrysoee.dk/editline/>`_ is optionally
used to provide command editing, tab completion, and history functionality at
the cvc5 prompt (when running in interactive mode).  Check your distribution for
a package named `libedit-dev`, `libedit-devel`, or similar.  Configure cvc5 with
``configure.sh --editline`` to build with this dependency.  Additionally,
to run tests related to interactive mode with this dependency, you will need
the Python module `pexpect <https://pexpect.readthedocs.io/en/stable/>`_.


Google Test Unit Testing Framework (Unit Tests)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`Google Test <https://github.com/google/googletest>`_ is required to optionally
run cvc5's unit tests (included with the distribution). 
See `Testing cvc5 <#testing-cvc5>`_
below for more details.


Language bindings
-----------------

cvc5 provides a complete and flexible C++ API (see ``examples/api`` for
examples). It further provides Java (see ``examples/SimpleVC.java`` and
``examples/api/java``) and Python (see ``examples/api/python``) API bindings.

Configure cvc5 with ``configure.sh --<lang>-bindings`` to build with language
bindings for ``<lang>``.


Dependencies for Language Bindings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Java

  - `JDK >= 1.8 <https://www.java.com>`_

- Python

  - `Cython <https://cython.org/>`_ >= 3.0.0
  - `pip <https://pip.pypa.io/>`_ >= 23.0
  - `pytest <https://docs.pytest.org/en/6.2.x/>`_
  - `repairwheel <https://github.com/jvolkman/repairwheel>`_ >= 0.3.1
  - `setuptools <https://setuptools.pypa.io/>`_ >= 66.1.0
  - The source for the `pythonic API <https://github.com/cvc5/cvc5_pythonic_api>`_

If ``--auto-download`` is given, the Python modules will be installed automatically in
a virtual environment if they are missing. To install the modules globally and skip
the creation of the virtual environment, configure cvc5 with ``./configure.sh --no-pyvenv``.

If configured with ``--pythonic-path=PATH``, the build system will expect the Pythonic API's source to be at ``PATH``.
Otherwise, if configured with ``--auto-download``, the build system will download it.

Installing the Python bindings after building from source requires a Python environment with
pip version 20.3 or higher.

If you're interested in helping to develop, maintain, and test a language
binding, please contact the cvc5 team via `our issue tracker
<https://github.com/cvc5/cvc5/issues>`_.


Building the API Documentation
------------------------------

Building the API documentation of cvc5 requires the following dependencies:

- `Doxygen <https://www.doxygen.nl>`_
- `Sphinx <https://www.sphinx-doc.org>`_,
  `sphinx-rtd-theme <https://sphinx-rtd-theme.readthedocs.io/>`_,
  `sphinx-tabs <https://sphinx-tabs.readthedocs.io/>`_,
  `sphinxcontrib-bibtex <https://sphinxcontrib-bibtex.readthedocs.io>`_,
  `sphinxcontrib-programoutput <https://sphinxcontrib-programoutput.readthedocs.io>`_
- `Breathe <https://breathe.readthedocs.io>`_

If ``--auto-download`` is given, Sphinx, the Sphinx extensions, and Breathe will be installed
automatically in a virtual Python environment if they are missing. To install the modules globally and skip
the creation of the virtual environment, configure cvc5 with ``./configure.sh --no-pyvenv``.

To build the documentation, configure cvc5 with ``./configure.sh --docs`` and
run ``make docs`` from within the build directory.

The API documentation can then be found at
``<build_dir>/docs/sphinx/index.html``.

To build the documentation for GitHub pages, change to the build directory and
call ``make docs-gh``. The content of directory ``<build_dir>/docs/sphinx-gh``
can then be copied over to GitHub pages.


Building the Examples
---------------------

See ``examples/README.md`` for instructions on how to build and run the
examples.


.. _testing-cvc5:

Testing cvc5
------------

We use ``ctest`` as test infrastructure. For all command-line options of ctest,
see ``ctest -h``. Some useful options are:

.. code::

    ctest -R <regex>           # run all tests with names matching <regex>
    ctest -E <regex>           # exclude tests with names matching <regex>
    ctest -L <regex>           # run all tests with labels matching <regex>
    ctest -LE <regex>          # exclude tests with labels matching <regex>
    ctest                      # run all tests
    ctest -jN                  # run all tests in parallel with N threads
    ctest --output-on-failure  # run all tests and print output of failed tests

We have 3 categories of tests:

- **api tests** in directory ``test/api`` (label: **api**)
- **regression tests** (5 levels) in directory ``test/regress`` (label: 
  **regressN** with N the regression level)
- **unit tests** in directory ``test/unit`` (label: **unit**)


Testing API Tests
^^^^^^^^^^^^^^^^^

The API tests are not built by default.

.. code::

    make apitests                         # build and run all API C++ tests
    make capitests                        # build and run all API C tests
    make <api_test>                       # build test/api/cpp/<api_test>.cpp
    make capi_<api_test>                  # build test/api/c/<api_test>.c
    ctest api/cpp/<api_test>              # run test/api/cpp/<api_test><.ext>
    ctest api/c/capi_<api_test>           # run test/api/cpp/capi_<api_test><.ext>

All API test binaries are built into ``<build_dir>/bin/test/api``.

We use prefix ``api/cpp/`` + ``<api_test>`` (for ``<api_test>`` in ``test/api/cpp``)
and ``api/c/`` + ``capi_<api_test>`` (for ``<api_test>`` in ``test/api/c``)
as test target name.

.. code::

    make ouroborous                       # build test/api/cpp/ouroborous.cpp
    make capi_ouroborous                  # build test/api/c/ouroborous.c
    ctest -R ouroborous                   # run all tests that match '*ouroborous*'
                                          # > runs api/cpp/ouroborous, api/c/capi_ouroborous
    ctest -R ouroborous$                  # run all tests that match '*ouroborous'
                                          # > runs api/cpp/ouroborous, api/c/capi_ouroborous
    ctest -R api/cpp/ouroborous$          # run all tests that match '*api/cpp/ouroborous'
                                          # > runs api/cpp/ouroborous


Testing Unit Tests
^^^^^^^^^^^^^^^^^^

The unit tests are not built by default.

Note that cvc5 can only be configured with unit tests in non-static builds with
assertions enabled (e.g. ``./configure.sh --unit-testing --assertions``).

.. code::

    make units                            # build and run all unit tests
    make <unit_test>                      # build test/unit/<subdir>/<unit_test>.<ext>
    ctest unit/<subdir>/<unit_test>       # run test/unit/<subdir>/<unit_test>.<ext>

All unit test binaries are built into ``<build_dir>/bin/test/unit``.

We use prefix ``unit/`` + ``<subdir>/`` + ``<unit_test>`` (for ``<unit_test>``
in ``test/unit/<subdir>``) as test target name.

.. code::

    make map_util_black                   # build test/unit/base/map_util_black.cpp
    ctest -R map_util_black               # run all tests that match '*map_util_black*'
                                          # > runs unit/base/map_util_black
    ctest -R base/map_util_black$         # run all tests that match '*base/map_util_black'
                                          # > runs unit/base/map_util_black
    ctest -R unit/base/map_util_black$    # run all tests that match '*unit/base/map_util_black'
                                          # > runs unit/base/map_util_black


Testing Regression Tests
^^^^^^^^^^^^^^^^^^^^^^^^

We use prefix ``regressN/`` + ``<subdir>/`` + ``<regress_test>`` (for
``<regress_test>`` in level ``N`` in ``test/regress/regressN/<subdir>``) as test
target name.

.. code::

    ctest -L regress                      # run all regression tests
    ctest -L regress0                     # run all regression tests in level 0
    ctest -L regress[0-1]                 # run all regression tests in level 0 and 1
    ctest -R regress                      # run all regression tests
    ctest -R regress0                     # run all regression tests in level 0
    ctest -R regress[0-1]                 # run all regression tests in level 0 and 1
    ctest -R regress0/bug288b             # run all tests that match '*regress0/bug288b*'
                                          # > runs regress0/bug288b


Custom Targets
^^^^^^^^^^^^^^

All custom test targets build and run a preconfigured set of tests.

- ``make check [-jN] [ARGS=-jN]``
  The default build-and-test target for cvc5, builds and runs all examples,
  all system and unit tests, and regression tests from levels 0 to 2.

- ``make apitests [-jN] [ARGS=-jN]``
  Build and run all API tests.

- ``make units [-jN] [ARGS=-jN]``
  Build and run all unit tests.

- ``make regress [-jN] [ARGS=-jN]``
  Build and run regression tests from levels 0 to 2.

To build the tests without executing them, run `make build-tests`.

We use ``ctest`` as test infrastructure, and by default all test targets
are configured to **run** in parallel with the maximum number of threads
available on the system. Override with ``ARGS=-jN``.

Use ``-jN`` for parallel **building** with ``N`` threads.


Recompiling a specific cvc5 version with different LGPL library versions
------------------------------------------------------------------------

To recompile a specific static binary of cvc5 with different versions of the
linked LGPL libraries perform the following steps:

1. Make sure that you have installed the desired LGPL library versions.
   You can check the versions found by cvc5's build system during the configure
   phase.

2. Determine the commit sha and configuration of the cvc5 binary

.. code::
  
  cvc5 --show-config

3. Download the specific source code version:

.. code::
  
  wget https://github.com/cvc5/cvc5/archive/<commit-sha>.tar.gz

4. Extract the source code

.. code::
  
  tar xf <commit-sha>.tar.gz

5. Change into source code directory

.. code::
  
  cd cvc5-<commit-sha>

6. Configure cvc5 with options listed by ``cvc5 --show-config``

.. code::
  
  ./configure.sh --static <options>

7. Follow remaining steps from `build instructions <#building-cvc5>`_
