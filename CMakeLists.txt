#General CMake intro: http://derekmolloy.ie/hello-world-introductions-to-cmake/
#Boost and CMake: https://cmake.org/cmake/help/v3.5/module/FindBoost.html

#set(Boost_DEBUG 1) 

cmake_minimum_required(VERSION 3.5.1)
set (CMAKE_CXX_STANDARD 14)
project(STC)

find_package(Threads)

set(Boost_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(Boost_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
find_package(Boost 1.62.0 COMPONENTS thread system filesystem program_options REQUIRED)

if(Boost_FOUND)
	include_directories(${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})
	
	# Net static library
	set(NET_SOURCES src/common/net/Server.cpp 
			src/common/net/Client.cpp 
			src/common/net/Session.cpp 
			src/common/net/ConnectionManager.cpp)
					
	add_library(STC_Net STATIC ${NET_SOURCES})
	# Client application
	set(CLIENT_SOURCES src/client/Main.cpp)
	add_executable(STC_Client ${CLIENT_SOURCES})
	# Server application
	set(LAUNCHSERVER_SOURCES src/launchserver/Main.cpp)
	add_executable(STC_LaunchServer ${LAUNCHSERVER_SOURCES})
		
	target_link_libraries(STC_Client ${Boost_LIBRARIES})
	target_link_libraries(STC_Client ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(STC_Client STC_Net)
	target_link_libraries(STC_LaunchServer ${Boost_LIBRARIES})
	target_link_libraries(STC_LaunchServer ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(STC_LaunchServer STC_Net)
endif()


