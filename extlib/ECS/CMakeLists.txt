cmake_minimum_required (VERSION 2.8.11)
project (ECS)
include(CheckCXXCompilerFlag)

set(ECS_BUILD_STATIC ON)
set(ECS_BUILD_SHARED OFF)
option(ECS_BUILD_EXAMPLES OFF)
option(ECS_BUILD_TESTS OFF)
message(${CMAKE_CXX_FLAGS})
add_subdirectory("lib")

