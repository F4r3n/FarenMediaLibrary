cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()


file(GLOB SOURCES "lib/src/*.cpp")
include_directories(lib/include)

find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)

set( SOL_INCLUDE_DIR "extlib/sol2")
set( STB_INCLUDE_DIR "extlib/stb")
set( LUA_INCLUDE_DIR "/usr/local/include")
set( LUA_LIBRARY_DIR "/usr/local/lib")
include_directories(${SOL_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries (${PROJECT_NAME} ${LUA_LIBRARY} lua)

if(NOT GLEW_FOUND)
message(FATAL_ERROR "GLEW not found!")
endif(NOT GLEW_FOUND)
include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})

if (GLFW_FOUND)
  include_directories(${GLFW_INCLUDE_DIRS})
  target_link_libraries (${PROJECT_NAME} ${GLFW_LIBRARIES})
endif (GLFW_FOUND)


