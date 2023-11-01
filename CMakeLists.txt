macro(SET_COMPILER_OPTIONS)
include(CheckCXXCompilerFlag)
endmacro(SET_COMPILER_OPTIONS)


cmake_minimum_required(VERSION 3.16)

project("FML")
option(ENABLE_VULKAN "Build with vulkan" ON)
option(ENABLE_ASSIMP "Build with assimp" OFF)

SET_COMPILER_OPTIONS()

add_subdirectory("extlib")
add_subdirectory("lib")
add_subdirectory("gui")
add_subdirectory("engine")
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory("tests")
endif(ENABLE_TESTS)