cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME} CXX)
include(CheckCXXCompilerFlag)
SET_COMPILER_OPTIONS()

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)
option(FML_BUILD_STATIC ON)
option(FML_BUILD_SHARED OFF)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories("include")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
	add_definitions(-DEMSCRIPTEN)
	set(FML_BUILD_STATIC ON CACHE BOOL "" FORCE)
	set(FML_BUILD_SHARED OFF CACHE BOOL "" FORCE)
	set(LINK_FLAGS "${LINK_FLAGS} -s USE_SDL=2")
else(EMSCRIPTEN)

endif(EMSCRIPTEN)
#Opengl
find_package(OpenGL)

#OpenAL
set(OPENAL_LIBRARIES OpenAL32)
link_directories(${FML_EXTLIB_LIB}/openal)
include_directories(${FML_EXTLIB}/openal/include)

#GLEW
set(GLEW_LIBRARIES glew)
link_directories(${FML_EXTLIB_LIB}/glew)
include_directories(${FML_EXTLIB}/glew/include)

#SDL2
include_directories(${FML_EXTLIB}/SDL2/include)
link_directories(${FML_EXTLIB_LIB}/SDL2)

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

#BOX2D
include_directories(${FML_EXTLIB}/Box2D/)
link_directories(${FML_EXTLIB_LIB}/Box2D)

#Assimp
include_directories(${FML_EXTLIB}/assimp/include)
link_directories(${FML_EXTLIB_LIB}/assimp/code/)

set(LIB_NAME ${PROJECT_NAME}_s)

    add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})
	target_link_libraries(${LIB_NAME} freetype)
	target_link_libraries(${LIB_NAME} ECS)
	target_link_libraries(${LIB_NAME} lua)
	target_link_libraries(${LIB_NAME} debug SDL2d optimized SDL2)
	target_link_libraries(${LIB_NAME} ${OPENGL_LIBRARIES} )
	target_link_libraries(${LIB_NAME} ${GLEW_LIBRARIES})
	target_link_libraries(${LIB_NAME} ${OPENAL_LIBRARIES})
	target_link_libraries(${LIB_NAME} Box2D)
    target_link_libraries(${LIB_NAME} assimp)
    target_link_libraries(${LIB_NAME} ${CMAKE_DL_LIBS})

	if(CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(${LIB_NAME} gcc_s stdc++)
	endif(CMAKE_COMPILER_IS_GNUCXX)


#INSTALL( DIRECTORY ${LIB_NAME} DESTINATION /usr/local/lib/fml/ )




