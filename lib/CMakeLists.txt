cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME} CXX)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -g -Wall -O3")



file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(include)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua 5.3 REQUIRED)
find_package(Box2D REQUIRED)
if(UNIX)
FIND_PACKAGE( PkgConfig REQUIRED )
pkg_search_module( GLFW3 3.3 REQUIRED glfw3 ) 
endif(UNIX)
if(WIN32)

find_package(GLFW3 3.3 REQUIRED glfw3)
endif(WIN32)


set( SOL_INCLUDE_DIR "../extlib/sol2")
set( STB_INCLUDE_DIR "../extlib/stb")
set( GLM_INCLUDE_DIR "../extlib/glm")
set( JSON_INCLUDE_DIR ../extlib/rapidjson/include)

set( ENTITY_INCLUDE_DIR "../extlib/ECS")

set(FREETYPE_INCLUDE_DIR "../extlib/freetype2/include")
set(FREETYPE_LIBRARY_DIR "../extlib/freetype2/build")
set(IMGUI_INCLUDE_DIR "../extlib/imgui")

include_directories(${GLFW3_INCLUDE_DIR} )
include_directories(${ENTITY_INCLUDE_DIR}/lib/include )
include_directories(${SOL_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIR})
include_directories(${IMGUI_INCLUDE_DIR})
include_directories(${JSON_INCLUDE_DIR})
link_directories(${FREETYPE_LIBRARY_DIR} freetype)
link_directories(${ENTITY_INCLUDE_DIR}/build/lib)



add_library(${PROJECT_NAME} SHARED ${SOURCES})
if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(${PROJECT_NAME} gcc gcc_s stdc++)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES} ${GLFW3_LIBRARIES} ${BOX2D_LIBRARIES}  ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ECS freetype openal sndfile)







