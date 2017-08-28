cmake_minimum_required (VERSION 3.0)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall -O3")
option(EMSCRIPTEN OFF)
set(V_EMSCRIPTEN ${EMSCRIPTEN})
if(EMSCRIPTEN)
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake)
set(LINK_FLAGS "${LINK_FLAGS} -s FULL_ES3=1")
endif(EMSCRIPTEN)

add_subdirectory("extlib")
add_subdirectory("lib")

if(NOT EMSCRIPTEN)
#add_subdirectory("gui")
else()

endif()


