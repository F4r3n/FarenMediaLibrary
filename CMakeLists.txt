cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME})
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -g -Wall ")


file(GLOB SOURCES "lib/src/*.cpp")
include_directories(lib/include)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

FIND_PACKAGE( PkgConfig REQUIRED )
pkg_search_module( GLFW3 3.3 REQUIRED glfw3 ) # sets GLFW3 as prefix for glfw vars

set( SOL_INCLUDE_DIR "extlib/sol2")
set( STB_INCLUDE_DIR "extlib/stb")
set( GLM_INCLUDE_DIR "extlib/glm")

set( LUA_INCLUDE_DIR "/usr/local/include")
set( LUA_LIBRARY_DIR "/usr/local/lib")

include_directories(${GLFW3_INCLUDE_DIR} )
include_directories(${SOL_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})

add_library(${PROJECT_NAME} SHARED ${SOURCES})
if(CMAKE_COMPILER_IS_GNUCXX)
target_link_libraries(${PROJECT_NAME} gcc gcc_s )
endif()
target_link_libraries (${PROJECT_NAME} ${LUA_LIBRARY} lua)
target_link_libraries( ${PROJECT_NAME} ${GLFW3_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})





