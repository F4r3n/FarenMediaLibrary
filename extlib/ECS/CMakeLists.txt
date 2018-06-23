cmake_minimum_required (VERSION 2.8.11)
project (ECS)
include(CheckCXXCompilerFlag)

option(BUILD_STATIC OFF)
option(BUILD_SHARED ON)
option(BUILD_EXAMPLES OFF)
option(BUILD_TESTS OFF)
add_subdirectory("lib")

