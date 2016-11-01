## STC

# Project structure
In the CMakeLists.txt it is expected that a boost directory is present with boost installation since BOOST_ROOT_DIR points to <STC>/boost.

It is also expected that grpc and protobuf installations are present.
An alternative to the existing FindProtobuf is provided under <STC>/FindProtobuf.cmake, this file
also builds the required grpc files using the protoc compiler and grpc plugin.

# How to build: 

1. Get Boost. Install in <STC>/boost
2. Get gRPC. Install

mkdir <some dir>
cd <some dir>
cmake ..
make

# Build Doxygen

doxygen doxyconfig

