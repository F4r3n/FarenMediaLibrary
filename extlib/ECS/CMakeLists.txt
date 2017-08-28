cmake_minimum_required (VERSION 2.8.11)
project (ECS)
include(CheckCXXCompilerFlag)

option(ECS_BUILD_STATIC OFF)
option(ECS_BUILD_SHARED ON)
option(ECS_BUILD_EXAMPLES OFF)
option(ECS_BUILD_TESTS OFF)
add_subdirectory("lib")

if(ECS_BUILD_EXAMPLES)
add_subdirectory("sample")
endif(ECS_BUILD_EXAMPLES)

if(ECS_BUILD_TESTS)
add_subdirectory("Tests")
endif(ECS_BUILD_TESTS)
