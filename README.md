# SNES to 3DS Graphics Translator

Translation layer for running SNES graphics on the 3DS.

Note that this is NOT intended to be a SNES emulator for 3DS - it is only a translation layer intended to simulate how SNES graphics are rendered.

This project was specifically created to aid in the [native 3DS port of Zelda 3](https://github.com/padinadrian/zelda3-3ds).

## Building and Running Tests

### Dependencies
The following programs/libraries are needed to build from source and run the
unit tests:
* C++ compiler (such as g++)
* python
* make
* cmake
* googletest (aka gtest)

You can install the dependencies with the following command:

On Ubuntu:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libgtest-dev python
```

### Building with Make
This project uses CMake to generate the build system. Included in the repository
is a script to automate the CMake generation process (see [build.sh](build.sh))

To build the project:
```bash
# 1. Clone the repository:
git clone https://github.com/padinadrian/snes-3ds-graphics.git

# 2. Run the generation script:
cd snes-3ds-graphics
./build.sh

#3. Run make to build all:
cd build
make
```

This will place the unit tests in the bin folder at `bin/run_tests`

## Running unit tests
The unit tests for this project use gtest, which needs to be installed separately (see Dependencies above).

You can run the tests directly from the build folder:
```bash
bin/run_tests
```

Or, you can also use this convenience script which automatically rebuilds the
tests before running:
```bash
./run_tests.sh
```
