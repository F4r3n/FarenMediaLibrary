cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenEngine")
project(${PROJECT_NAME})
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -g -Wall -O3")


add_subdirectory("lib")
add_subdirectory("gui")


