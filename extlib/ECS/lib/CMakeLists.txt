cmake_minimum_required (VERSION 2.8.11)
project (ECS CXX)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")


include_directories("include")
file(GLOB SOURCES "src/*.cpp")
if(ECS_BUILD_STATIC)
add_library(ECS_static STATIC ${SOURCES})


endif(ECS_BUILD_STATIC)

if(ECS_BUILD_SHARED)
add_library(ECS SHARED ${SOURCES})


endif(ECS_BUILD_SHARED)

