## STC

# How to build the project: 

1. Build & install following Boost: programoptions, ASIO (used by Autobahn).
2. Build & install gRPC. Required plugins are c++ and Ruby.

An alternative to the existing FindProtobuf is provided under <STC>/FindProtobuf.cmake, this file
also builds the required grpc files using the protoc compiler and grpc plugin.

3. Build & install authobahn. (WAMP impl for cpp).
For this msgpack also has to be installed; use installation description in Autobahn
need to checkout some specific branch of msgpack in order for autobahn to compile.

4. Install crossbar.io
A glorious open source WAMP router used to route messages between the executables.
Excellent installation instructions available at their website.

5. Build the project
mkdir <STC>/build
cd <STC>/build
cmake ..
make

6. Run the project
The crossbar.io config file in build/ includes commands to also start the STC_Server executable
when crossbar is launched.

cd <build>
crossbar start

# Build Doxygen

doxygen doxyconfig

