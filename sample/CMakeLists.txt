cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "TestFML")

project(${PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

file(GLOB SOURCES "src/*.cpp")

set(FML_DIR "../")
set(FML_INCLUDE_SOL ${FML_DIR}/extlib/sol2)
set(FML_INCLUDE_STB ${FML_DIR}/extlib/stb)
set(FML_INCLUDE_FREETYPE ${FML_DIR}/extlib/freetype2/include)
set(FML_INCLUDE ${FML_DIR}/lib/include)
set(FML_LIB ${FML_DIR}/build/)
message(${FML_LIB})

include_directories(${FML_INCLUDE})
include_directories(${FML_INCLUDE_STB})
include_directories(${FML_INCLUDE_SOL})
include_directories(${FML_INCLUDE_FREETYPE})

link_directories(${FML_LIB})

add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} FarenMediaLibrary)






