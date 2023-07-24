cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME} CXX)
include(CheckCXXCompilerFlag)
SET_COMPILER_OPTIONS()

set(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${PROJECT_BINARY_DIR})

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)
option(FML_BUILD_STATIC ON)
option(FML_BUILD_SHARED OFF)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")


#Bullet
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath BulletSoftBody)
include_directories(${FML_EXTLIB}/bullet3/src)
link_directories(${FML_EXTLIB_LIB}/bullet3)

#Opengl
find_package(OpenGL)

#OpenAL
include_directories(${FML_EXTLIB}/openal/include)
link_directories(${FML_EXTLIB_LIB}/openal/lib)


#GLEW
add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)
include_directories(${FML_EXTLIB}/glew/include)
link_directories(${FML_EXTLIB_LIB}/glew)

#SDL2
include_directories(${FML_EXTLIB}/SDL3/include)
link_directories(${FML_EXTLIB_LIB}/SDL3)

#sol2
include_directories(${FML_EXTLIB}/sol2)

#STB
include_directories(${FML_EXTLIB}/stb)

#RAPID_JSON
include_directories(${FML_EXTLIB}/json/include)

#ECS
include_directories(${FML_EXTLIB}/ECS/lib/include)
link_directories(${FML_EXTLIB_LIB}/ECS)

#FREETYPE
include_directories(${FML_EXTLIB}/freetype2/include)
link_directories(${FML_EXTLIB_LIB}/freetype2)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

#Assimp
include_directories(${FML_EXTLIB}/assimp/include)
link_directories(${FML_EXTLIB_LIB}/assimp/code/)

set(LIB_NAME ${PROJECT_NAME}_s)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
    
target_link_libraries(${LIB_NAME} freetype)
target_link_libraries(${LIB_NAME} ECS)
target_link_libraries(${LIB_NAME} lua)
target_link_libraries(${LIB_NAME} debug SDL3d optimized SDL3)
target_link_libraries(${LIB_NAME} glew_s)
target_link_libraries(${LIB_NAME} ${OPENGL_LIBRARIES} )
target_link_libraries(${LIB_NAME} OpenAL)
target_link_libraries(${LIB_NAME} assimp)
target_link_libraries(${LIB_NAME} ${BULLET_LIBRARIES})
target_link_libraries(${LIB_NAME} ${CMAKE_DL_LIBS})

if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(${LIB_NAME} gcc_s stdc++)
	target_link_libraries(${LIB_NAME} stdc++fs)
endif(CMAKE_COMPILER_IS_GNUCXX)


#INSTALL( DIRECTORY ${LIB_NAME} DESTINATION /usr/local/lib/fml/ )




