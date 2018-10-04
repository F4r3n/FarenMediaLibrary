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
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)


#Opengl
find_package(OpenGL)

#OpenAL
set(OPENAL_LIBRARIES OpenAl32)
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


