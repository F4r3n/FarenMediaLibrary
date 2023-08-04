cmake_minimum_required (VERSION 2.8.11)
project (ECS)
include(CheckCXXCompilerFlag)

set(ECS_BUILD_STATIC ON)
set(ECS_BUILD_SHARED OFF)
option(ECS_BUILD_EXAMPLES ON)
option(ECS_BUILD_TESTS OFF)
add_subdirectory("lib")
add_subdirectory("sample")



