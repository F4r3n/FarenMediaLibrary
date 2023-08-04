macro(SET_COMPILER_OPTIONS)
include(CheckCXXCompilerFlag)
if(MSVC)
	add_compile_options("/W3")
else(MSVC)
	add_compile_options("-Wall")
endif(MSVC)
endmacro(SET_COMPILER_OPTIONS)


cmake_minimum_required (VERSION 3.10)

project("FML")

SET_COMPILER_OPTIONS()

add_subdirectory("extlib")
add_subdirectory("lib")
add_subdirectory("engine")
add_subdirectory("gui")
