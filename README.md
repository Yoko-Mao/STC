## STC

# How to build the project: 

1. Build & install following Boost: programoptions, ASIO (used by Autobahn).
2. Build & install gRPC. Required plugins are c++ and Ruby.

mkdir <some dir>
cd <some dir>
cmake ..
make

An alternative to the existing FindProtobuf is provided under <STC>/FindProtobuf.cmake, this file
also builds the required grpc files using the protoc compiler and grpc plugin.

3. Build & install authobahn. (WAMP impl for cpp).
For this msgpack also has to be installed; use installation description in Autobahn
need to checkout some specific branch of msgpack in order for autobahn to compile.

# Build Doxygen

doxygen doxyconfig

