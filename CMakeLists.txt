#http://derekmolloy.ie/hello-world-introductions-to-cmake/

cmake_minimum_required(VERSION 3.5.1)
project(STC)
 
include_directories(include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
 
add_executable(STC ${SOURCES})



