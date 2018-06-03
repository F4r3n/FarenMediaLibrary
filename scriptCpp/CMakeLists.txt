cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "ScriptCpp")
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -fPIC -g")

file(GLOB SOURCES "src/*.cpp")
include_directories(include)

include_directories("include")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${FML_PATH}/build)


#OPENGL
find_package(OpenGL REQUIRED)

#OPENAL
find_package(OpenAL REQUIRED)
link_directories(${OPENAL_LIBRARIES_DIR})
include_directories(${OPENAL_INCLUDE_DIR})

#SDL2
include_directories(${FML_EXTLIB}/SDL2/include)
link_directories(${FML_EXTLIB_LIB}/SDL2/lib)
MESSAGE(STATUS ${FML_EXTLIB}/SDL2/include)
#GLEW
include_directories(${FML_EXTLIB}/glew/include)
link_directories(${FML_EXTLIB_LIB}/glew/lib)

#sol2
include_directories(${FML_EXTLIB}/sol2)

#STB
include_directories(${FML_EXTLIB}/stb)

#JSON
include_directories(${FML_EXTLIB}/json/include)

#ECS
include_directories(${FML_EXTLIB}/ECS/lib/include)
link_directories(${FML_EXTLIB_LIB}/ECS/lib)

#FREETYPE
include_directories(${FML_EXTLIB}/freetype2/include)
link_directories(${FML_EXTLIB_LIB}/freetype2/lib)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua/lib)

#BOX2D
include_directories(${FML_EXTLIB}/Box2D/)
link_directories(${FML_EXTLIB_LIB}/Box2D/lib)

#FML
include_directories(${FML_PATH}/lib/include)
link_directories(${FML_PATH}/build/lib)

set(LIB_NAME ${PROJECT_NAME})

add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary glew SDL2-2.0 lua_shared glew freetype openal ECS Box2D ${OPENGL_LIBRARIES})


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)


