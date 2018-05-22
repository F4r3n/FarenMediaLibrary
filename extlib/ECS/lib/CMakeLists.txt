cmake_minimum_required (VERSION 2.8.11)
project (ECS CXX)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")


include_directories("include")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

include_directories(${CMAKE_SOURCE_DIR}/extlib/json/include)
file(GLOB SOURCES "src/*.cpp")
if(BUILD_STATIC)
add_library(ECS STATIC ${SOURCES} ${HEADERS})
endif(BUILD_STATIC)

#if(BUILD_SHARED)
add_library(ECS SHARED ${SOURCES} ${HEADERS})
#endif(BUILD_SHARED)

