#General CMake intro: http://derekmolloy.ie/hello-world-introductions-to-cmake/
#Boost and CMake: https://cmake.org/cmake/help/v3.5/module/FindBoost.html

#set(Boost_DEBUG 1)
#set(Protobuf_DEBUG 1)

cmake_minimum_required(VERSION 3.5.1)
set (CMAKE_CXX_STANDARD 14)
project(STC)
#list(APPEND CMAKE_PREFIX_PATH /usr/local/lib)
include(FindProtobuf)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Threads)

set(Protobuf_SRC_ROOT_FOLDER ${CMAKE_SOURCE_DIR}/protobuf)
set(PROTOBUF_IMPORT_DIRS ${Protobuf_SRC_ROOT_FOLDER}/src) # Additional .proto files can be found here
find_package(Protobuf REQUIRED)

set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/boost)
find_package(Boost 1.62.0 COMPONENTS thread system filesystem program_options REQUIRED)

if(Boost_FOUND)
	message(${Boost_LIBRARIES})
	include_directories(${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

    # add_subdirectory(src/common/msg)
	# Net static library
	set(NET_SOURCES src/common/net/Server.cpp 
			src/common/net/Client.cpp 
			src/common/net/Session.cpp 
			src/common/net/ConnectionManager.cpp)

    file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/src/common/msg/*.proto")
    PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
    get_filename_component(ProtoSourcesNames ${ProtoSources} NAME)
    set(ProtoSources ${CMAKE_CURRENT_BINARY_DIR}/${ProtoSourcesNames})
    set_source_files_properties(${ProtoSources} PROPERTIES GENERATED TRUE)
    add_library(STC_Msg STATIC ${ProtoSources} ${ProtoHeaders})
    target_link_libraries(STC_Msg ${PROTOBUF_LIBRARY})

	add_library(STC_Net STATIC ${NET_SOURCES})
    target_link_libraries(STC_Net ${Boost_LIBRARIES})
	target_link_libraries(STC_Net STC_Msg)

    # Core static library
	set(CORE_SOURCES src/common/core/Core.cpp src/common/core/Core.cpp src/common/core/Core.h)
	add_library(STC_Core STATIC ${CORE_SOURCES})

	# Client application
	set(CLIENT_SOURCES src/client/Main.cpp)
	add_executable(STC_Client ${CLIENT_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})

	# Server application
	set(LAUNCHSERVER_SOURCES src/launchserver/Main.cpp)
	add_executable(STC_LaunchServer ${LAUNCHSERVER_SOURCES})

	target_link_libraries(STC_Client ${Boost_LIBRARIES})
	target_link_libraries(STC_Client ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(STC_Client STC_Core)
	target_link_libraries(STC_Client STC_Net)
    target_link_libraries(STC_Client STC_Msg)
	target_link_libraries(STC_LaunchServer ${Boost_LIBRARIES})
	target_link_libraries(STC_LaunchServer ${CMAKE_THREAD_LIBS_INIT})
	target_link_libraries(STC_LaunchServer STC_Core)
    target_link_libraries(STC_LaunchServer STC_Msg)
	target_link_libraries(STC_LaunchServer STC_Net)
endif()


