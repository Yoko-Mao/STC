#General CMake intro: http://derekmolloy.ie/hello-world-introductions-to-cmake/
#Boost and CMake: https://cmake.org/cmake/help/v3.5/module/FindBoost.html

#set(Boost_DEBUG 1) 

cmake_minimum_required(VERSION 3.5.1)
set (CMAKE_CXX_STANDARD 14)
project(STC)

find_package(Threads)

set(Boost_INCLUDE_DIR /home/yoko/STC/include)
set(Boost_LIBRARY_DIR /home/yoko/STC/lib)
find_package(Boost 1.62.0 COMPONENTS thread system filesystem program_options REQUIRED)

if(Boost_FOUND)
	#file(GLOB SOURCES "src/*.cpp")
	set(SOURCES src/Main.cpp src/net/Server.cpp src/net/Session.cpp src/net/Client.cpp)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(STC ${SOURCES})
	#target_link_libraries(STC $(Boost_LIBRARIES))
	target_link_libraries(STC ${Boost_LIBRARIES})
	#target_link_libraries(STC  Boost::thread Boost::system Boost::filesystem)
	target_link_libraries(STC ${CMAKE_THREAD_LIBS_INIT})
endif()

#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:



