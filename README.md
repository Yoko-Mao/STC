## STC

# Project structure
ROOT STC
|
|--boost
|
|--protobuf
|
|--src

In the CMakeLists.txt it is expected that a boost directory is present with boost installation.
BOOST_ROOT_DIR points to <STC>/boost.

It is also expected that protobuf installation is present (dynamic linking) in a subdir <STC>/protobuf.
Some variables of the CMakeLists.txt expect this directory to exist with the src tree and additional include .proto files present.

# How to build: 

1. Get Boost. Install in <STC>/boost
2. Get proto. Install in <STC>/protobuf

mkdir <some dir>
cd <some dir>
cmake ..
make

# Build Doxygen

doxygen doxyconfig

