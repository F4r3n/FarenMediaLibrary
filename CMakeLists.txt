macro(SET_COMPILER_OPTIONS)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

if(HAVE_STD_C14)
	add_compile_options("-std=c++14")
endif()

if(MSVC)
	add_compile_options("/W4")
else(MSVC)
		add_compile_options("-Wall")
endif(MSVC)
endmacro(SET_COMPILER_OPTIONS)


cmake_minimum_required (VERSION 3.0)

include(CheckCXXCompilerFlag)


SET_COMPILER_OPTIONS()


option(EMSCRIPTEN OFF)
set(V_EMSCRIPTEN ${EMSCRIPTEN})
if(EMSCRIPTEN)
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake)
set(LINK_FLAGS "${LINK_FLAGS} -s FULL_ES3=1")
endif(EMSCRIPTEN)

add_subdirectory("extlib")
add_subdirectory("lib")
add_subdirectory("gui")
add_subdirectory("scriptCpp")

