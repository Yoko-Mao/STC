## STC

# How to build the project: 

1. Build & install Boost libraries.
2. Build & install gRPC. Required plugins are c++ and Ruby.
An alternative to the existing FindProtobuf is provided under <STC>/FindProtobuf.cmake, this file
also builds the required grpc files using the protoc compiler and grpc plugin.

3. Build & install authobahn-cpp. (WAMP impl for cpp).
For this msgpack also has to be installed; use installation description in Autobahn
need to checkout some specific branch of the cloned msgpack git repo in order for autobahn to compile.

4. Install crossbar.io
A glorious open source WAMP router used to route messages between the executables.
Excellent installation instructions available at their website.

5. Build and SOCI.
Wrapper library for sqlite3 database access.

6. Build the project
mkdir <STC>/build
cd <STC>/build
cmake ..
make

7. Run the project
The crossbar.io config file in <STC> includes commands to also start the STC_Server executable
when crossbar is launched. This config file will also start the authenticator and database nodejs services.
Enter 'crossbar start' in <STC> directory.

# Build Doxygen
doxygen doxyconfig
