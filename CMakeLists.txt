#General CMake intro: http://derekmolloy.ie/hello-world-introductions-to-cmake/
#Boost and CMake: https://cmake.org/cmake/help/v3.5/module/FindBoost.html

#set(Boost_DEBUG 1)
#set(Protobuf_DEBUG 1)
project(STC)
include(FindPkgConfig)
include(FindProtobuf)
cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(PROTOBUF_IMPORT_DIRS /usr/local/include) # Additional .proto files can be found here
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror")
endif()
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(/usr/local/include)
find_package(Threads)
find_package(Protobuf REQUIRED)
find_package(Boost 1.62.0 COMPONENTS thread system filesystem program_options REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") # Needed to avoid compiler errors.
# No find_package(grpc) seems to exist yet, using FindPkgConfig to get correct linker flags for grpc.
# pgkconfig is created when building grpc from source.
pkg_check_modules(GRPC grpc++)
if(Boost_FOUND)
    include_directories(${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

    file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/src/common/msg/*.proto")
    PROTOBUF_GENERATE_CPP_RUBY(ProtoSources ProtoHeaders ${ProtoFiles})
    set_source_files_properties(${ProtoSources} PROPERTIES GENERATED TRUE)
    add_library(STC_Msg STATIC ${ProtoSources} ${ProtoHeaders})
    target_link_libraries(STC_Msg ${PROTOBUF_LIBRARY})
    target_link_libraries(STC_Msg ${GRPC_LDFLAGS})

    # Core static library
    set(CORE_SOURCES
        src/common/core/task/WorkOrder.cpp
        src/common/core/task/Thread.cpp
        src/common/core/task/WorkOrderQueue.cpp
        src/common/core/utilities/CommandLine.cpp)
    add_library(STC_Core STATIC ${CORE_SOURCES})

    # Server application
    set(SERVER_SOURCES 
        src/server/Lobby.cpp
        src/server/RPC.cpp)

    # Client application
    set(CLIENT_SOURCES
        src/client/RPC.cpp)

    add_executable(STC_Client src/client/Main.cpp ${CLIENT_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
    add_executable(STC_UnitTest src/MainUnitTest.cpp ${CLIENT_SOURCES} ${SERVER_SOURCES} ${PROTO_SRCS} ${PROTO_HDRS})
    add_executable(STC_Server src/server/Main.cpp ${SERVER_SOURCES})

    target_link_libraries(STC_Client ${Boost_LIBRARIES})
    target_link_libraries(STC_Client ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(STC_Client STC_Core)
    target_link_libraries(STC_Client STC_Msg)

    target_link_libraries(STC_Server ${Boost_LIBRARIES})
    target_link_libraries(STC_Server ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(STC_Server STC_Core)
    target_link_libraries(STC_Server STC_Msg)

    target_link_libraries(STC_UnitTest ${Boost_LIBRARIES})
    target_link_libraries(STC_UnitTest ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(STC_UnitTest STC_Core)
    target_link_libraries(STC_UnitTest STC_Msg)
endif()


