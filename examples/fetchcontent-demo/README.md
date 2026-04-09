# FetchContent Demo

This is a demonstration of using CMake's FetchContent to include cvc5 in a project.

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./demo
```

This should output the result of a simple UNSAT query using cvc5.