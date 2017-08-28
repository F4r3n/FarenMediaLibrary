cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME "FarenEngine")
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O2")

file(GLOB SOURCES "src/*.cpp")


include_directories(include)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua 5.3 REQUIRED)
find_package(Box2D REQUIRED)
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})



set( FML_PATH "../")
set( SOL_INCLUDE_DIR ${FML_PATH}extlib/sol2)
set( STB_INCLUDE_DIR ${FML_PATH}extlib/stb)
set( GLM_INCLUDE_DIR ${FML_PATH}extlib/glm)
set( JSON_INCLUDE_DIR ${FML_PATH}extlib/rapidjson/include)
set( ENTITY_INCLUDE_DIR ${FML_PATH}extlib/ECS)


set(FREETYPE_INCLUDE_DIR ${FML_PATH}extlib/freetype2/include)
set(FREETYPE_LIBRARY_DIR ${FML_PATH}extlib/freetype2/build)

include_directories(${JSON_INCLUDE_DIR})

include_directories(${ENTITY_INCLUDE_DIR}/lib/include )
include_directories(${SOL_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${IMGUI_INCLUDE_DIR})
include_directories(${FML_PATH}/extlib/imgui)
include_directories(${FML_PATH}/lib/include)
include_directories(include)

link_directories(${ENTITY_INCLUDE_DIR}/build/lib)
link_directories(${FML_PATH}build/lib)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}  FarenMediaLibrary )

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)


