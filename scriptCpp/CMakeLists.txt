cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "ScriptCpp")
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -fPIC -g")

file(GLOB Scripts_SRC "src/scripts/*.cpp")

include_directories("include")
file(GLOB_RECURSE Scripts_HDR "include/scripts/*.h" "include/scripts/*.hpp")

set(General_SRC
        src/interface.cpp
        src/implementation.cpp
)
set(General_HDR
        include/interface.hpp
)



set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${FML_PATH}/build)


#OPENGL
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

	#GLEW
	include_directories(${FML_EXTLIB}/glew/include)
	link_directories(${CMAKE_BINARY_DIR}/bin)
ENDIF()

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

add_library(${LIB_NAME} SHARED
            ${General_SRC} ${General_HDR}
            ${Scripts_SRC} ${Scripts_HDR}

)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s SDL2 lua ${GLEW_LIBRARIES} freetype ${OPENAL_LIBRARIES} ECS Box2D assimp ${OPENGL_LIBRARIES})


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)


