cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "FarenEngine")
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O2")

file(GLOB SOURCES "src/*.cpp")
include_directories(include)

set(FML_PATH /home/guillaume2/project/fml/FarenMediaLibrary)
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

#GLEW
include_directories(${FML_EXTLIB}/glew/include)
link_directories(${FML_EXTLIB_LIB}/glew/lib)
MESSAGE(STATUS ${FML_EXTLIB_LIB}/glew/lib)
#sol2
include_directories(${FML_EXTLIB}/sol2)

#STB
include_directories(${FML_EXTLIB}/stb)

#RAPID_JSON
include_directories(${FML_EXTLIB}/json/include)

#ECS
include_directories(${FML_EXTLIB}/ECS/lib/include)
link_directories(${FML_EXTLIB_LIB}/ECS/lib)

#FREETYPE
include_directories(${FML_EXTLIB}/freetype2/include)
link_directories(${FML_EXTLIB_LIB}/freetype2/lib)

#LUA
MESSAGE(STATUS ${FML_EXTLIB_LIB}/lua)
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua/lib)

#BOX2D
include_directories(${FML_EXTLIB}/Box2D/)
link_directories(${FML_EXTLIB_LIB}/Box2D/lib)

#FML
include_directories(${FML_PATH}/lib/include)
link_directories(${FML_PATH}/build/lib)

#IMGUI
include_directories(${FML_EXTLIB}/imgui/)


add_executable(${PROJECT_NAME} ${SOURCES})

if(${EMSCRIPTEN})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_FREETYPE=1 -s FULL_ES2=1 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -o ${PROJECT_NAME}.html -v")
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s lua_shared openal ECS_static Box2D )
else ()
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary glew SDL2-2.0 lua_shared glew freetype openal ECS Box2D ${OPENGL_LIBRARIES})
endif()

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)


