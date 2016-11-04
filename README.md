## STC

# How to build: 

1. Build & install following Boost libs: program options
2. Build & install gRPC

mkdir <some dir>
cd <some dir>
cmake ..
make

An alternative to the existing FindProtobuf is provided under <STC>/FindProtobuf.cmake, this file
also builds the required grpc files using the protoc compiler and grpc plugin.

# Build Doxygen

doxygen doxyconfig

