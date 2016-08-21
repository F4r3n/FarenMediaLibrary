cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


file(GLOB SOURCES "lib/src/*.cpp")
include_directories(lib/include)

find_package(SOIL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)



add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (SOIL_FOUND)
  include_directories(${SOIL_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${SOIL_LIBRARIES})

elseif(NOT SOIL_FOUND) 
message("error soil not found") 
endif (SOIL_FOUND)

if(NOT GLEW_FOUND)
message(FATAL_ERROR "GLEW not found!")
endif(NOT GLEW_FOUND)
include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})

if (GLFW_FOUND)
  include_directories(${GLFW_INCLUDE_DIRS})
  target_link_libraries (${PROJECT_NAME} ${GLFW_LIBRARIES})
endif (GLFW_FOUND)


