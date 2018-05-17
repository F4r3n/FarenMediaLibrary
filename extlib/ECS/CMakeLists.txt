cmake_minimum_required (VERSION 2.8.11)
project (ECS)
include(CheckCXXCompilerFlag)

option(BUILD_STATIC OFF)
option(BUILD_SHARED ON)
option(BUILD_EXAMPLES OFF)
option(BUILD_TESTS OFF)
add_subdirectory("lib")

#if(BUILD_EXAMPLES)
#add_subdirectory("sample")
#endif(BUILD_EXAMPLES)

#MESSAGE(STATUS ${BUILD_SHARED})
#if(BUILD_TESTS)
#add_subdirectory("Tests")
#endif(BUILD_TESTS)
