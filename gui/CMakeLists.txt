cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "FarenEngine")
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

file(GLOB SOURCES_IMGUI "extlib/imgui/*.cpp")
file(GLOB HEADERS_IMGUI "extlib/imgui/*.hpp" "extlib/imgui/*.h")
include_directories("extlib")


file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

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
MESSAGE(STATUS ${FML_EXTLIB}/ECS/lib/include)
#FREETYPE
include_directories(${FML_EXTLIB}/freetype2/include)
link_directories(${FML_EXTLIB_LIB}/freetype2/lib)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua/lib)

#BOX2D
include_directories(${FML_EXTLIB}/Box2D/)
link_directories(${FML_EXTLIB_LIB}/Box2D/lib)

#Assimp
include_directories(${FML_EXTLIB}/assimp/include)
link_directories(${FML_EXTLIB_LIB}/assimp/lib/)

#FML
include_directories(${FML_PATH}/lib/include)
link_directories(${FML_PATH}/build/lib)

#IMGUI
include_directories(${FML_EXTLIB}/imgui/)



add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SOURCES_IMGUI} ${HEADERS_IMGUI})
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary glew SDL2-2.0 lua_shared glew freetype openal ECS Box2D assimp ${OPENGL_LIBRARIES})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



