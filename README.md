## STC

# How to build: 

cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Debug or cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release
make -C build/

# Build Doxygen

doxygen doxyconfig

