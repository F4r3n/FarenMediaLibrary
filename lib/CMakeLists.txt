cmake_minimum_required (VERSION 3.0)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME} CXX)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -W1")
set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${FML_PATH}/build/extlib)
option(FML_BUILD_STATIC ON)
option(FML_BUILD_SHARED OFF)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories("include")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")


if(EMSCRIPTEN)
	add_definitions(-DEMSCRIPTEN)
	set(FML_BUILD_STATIC ON CACHE BOOL "" FORCE)
	set(FML_BUILD_SHARED OFF CACHE BOOL "" FORCE)
	set(LINK_FLAGS "${LINK_FLAGS} -s USE_SDL=2")
else(EMSCRIPTEN)

endif(EMSCRIPTEN)
IF (MSVC)
	find_package(OpenGL)
	find_package(OpenAl)
	if(NOT OpenAl_FOUND)
		set (OPENAL_LIBRARIES_DIR "${FML_EXTLIB}/openal/windows/libs/Win64" CACHE PATH "Openal lib" FORCE)
		set (OPENAL_INCLUDE_DIR "${FML_EXTLIB}/openal/windows/include" CACHE PATH "Openal include" FORCE)
		set (OPENAL_LIBRARIES "openal32" CACHE STRING "Include dir" FORCE)

	endif()
	link_directories(${OPENAL_LIBRARIES_DIR})
	include_directories(${OPENAL_INCLUDE_DIR})

	#GLEW
	find_package(GLEW)
	if(NOT GLEW_FOUND)
		set (GLEW_LIBRARIES_DIR "${FML_EXTLIB}/glew/lib/Release/x64" CACHE STRING "Lib dir" FORCE)
		set (GLEW_INCLUDE_DIR "${FML_EXTLIB}/glew/include" CACHE PATH "Include dir" FORCE)
		set (GLEW_LIBRARIES "glew32" CACHE STRING "Include dir" FORCE)
	endif()

	include_directories(${GLEW_INCLUDE_DIR})
	link_directories(${GLEW_LIBRARIES_DIR})
ELSE()
	
	#OPENGL
	find_package(OpenGL REQUIRED)
	
	#OPENAL
	find_package(OpenAL REQUIRED)
	link_directories(${OPENAL_LIBRARIES_DIR})
	include_directories(${OPENAL_INCLUDE_DIR})

	find_package(GLEW)
	link_directories(${GLEW_LIBRARIES_DIR})
	include_directories(${GLEW_INCLUDE_DIR})
ENDIF()

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

if(BUILD_STATIC_FML)
set(LIB_NAME ${PROJECT_NAME}_s)

    add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})
	target_link_libraries(${LIB_NAME} freetype)
	target_link_libraries(${LIB_NAME} ECS)
	target_link_libraries(${LIB_NAME} lua)
	target_link_libraries(${LIB_NAME} SDL2)
	target_link_libraries(${LIB_NAME} ${OPENGL_LIBRARIES} )
	target_link_libraries(${LIB_NAME} ${GLEW_LIBRARIES})
	target_link_libraries(${LIB_NAME} ${OPENAL_LIBRARIES})
	target_link_libraries(${LIB_NAME} Box2D)
    target_link_libraries(${LIB_NAME} assimp)
    target_link_libraries(${LIB_NAME} ${CMAKE_DL_LIBS})

	if(CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(${LIB_NAME} gcc_s stdc++)
	endif(CMAKE_COMPILER_IS_GNUCXX)
endif(BUILD_STATIC_FML)

if(BUILD_SHARED_FML)
set(LIB_NAME ${PROJECT_NAME})

    add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})
	target_link_libraries(${LIB_NAME} freetype)
	target_link_libraries(${LIB_NAME} ECS)
	target_link_libraries(${LIB_NAME} lua_shared)
	target_link_libraries(${LIB_NAME} SDL2)
	target_link_libraries(${LIB_NAME} ${OPENGL_LIBRARIES})
	target_link_libraries(${LIB_NAME} ${GLEW_LIBRARIES})
	target_link_libraries(${LIB_NAME} ${OPENAL_LIBRARIES})
	target_link_libraries(${LIB_NAME} Box2D_shared)
    target_link_libraries(${LIB_NAME} assimp)
    target_link_libraries(${LIB_NAME} ${CMAKE_DL_LIBS})

	if(CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(${LIB_NAME} gcc stdc++)
	endif(CMAKE_COMPILER_IS_GNUCXX)
endif(BUILD_SHARED_FML)

#INSTALL( DIRECTORY ${LIB_NAME} DESTINATION /usr/local/lib/fml/ )




