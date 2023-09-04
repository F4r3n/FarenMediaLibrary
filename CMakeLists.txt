macro(SET_COMPILER_OPTIONS)
include(CheckCXXCompilerFlag)
endmacro(SET_COMPILER_OPTIONS)


cmake_minimum_required(VERSION 3.15)

project("FML")
option(WANT_VULKAN "Build with vulkan" ON)
SET_COMPILER_OPTIONS()

add_subdirectory("extlib")
add_subdirectory("lib")
add_subdirectory("engine")
add_subdirectory("gui")
