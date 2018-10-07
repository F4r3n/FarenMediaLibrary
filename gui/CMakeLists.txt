cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "FarenEngine")
project(${PROJECT_NAME})

include_directories("extlib")
set(SOURCES ${SOURCES} 
	src/imgui_impl_sdl_gl3.cpp

	src/imgui_impl_sdl.cpp
	src/imgui_impl_opengl3.cpp
	extlib/imgui/imgui.cpp
	extlib/imgui/imgui_draw.cpp
	extlib/imgui/imgui_widgets.cpp
	extlib/imgui/imgui_demo.cpp

	src/main.cpp
	src/MainWindow.cpp
	src/GameView.cpp
	src/inspector/Inspector.cpp
	src/inspector/MaterialInspector.cpp
	src/inspector/MeshInspector.cpp
	src/inspector/pointlightinspector.cpp
	src/inspector/scriptmanagerinspector.cpp
	src/inspector/TransformInspector.cpp
	src/debuglogger.cpp
	src/dialogfilebrowser.cpp

	)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)


#Opengl
find_package(OpenGL)

#OpenAL
set(OPENAL_LIBRARIES OpenAL32)
link_directories(${FML_EXTLIB_LIB}/openal/lib)
include_directories(${FML_EXTLIB}/openal/include)

#GLEW
set(GLEW_LIBRARIES glew)
link_directories(${FML_EXTLIB_LIB}/glew)
include_directories(${FML_EXTLIB}/glew/include)

#SDL2
include_directories(${FML_EXTLIB}/SDL2/include)
link_directories(${FML_EXTLIB_LIB}/SDL2/lib)

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
#include_directories(${FML_EXTLIB}/lua/src)
#link_directories(${FML_EXTLIB_LIB}/lua/lib)

#BOX2D
#include_directories(${FML_EXTLIB}/Box2D/)
#link_directories(${FML_EXTLIB_LIB}/Box2D/lib)

#Assimp
include_directories(${FML_EXTLIB}/assimp/include)
link_directories(${FML_EXTLIB_LIB}/assimp/lib/)

#FML
include_directories(${FML_PATH}/lib/include)
link_directories(${FML_PATH}/build/lib)

#IMGUI
include_directories(${FML_EXTLIB}/imgui/)



add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s ${GLEW_LIBRARIES} freetype ${OPENAL_LIBRARIES} assimp ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} debug SDL2d optimized SDL2)
target_link_libraries(${PROJECT_NAME} ECS)
#target_link_libraries(${PROJECT_NAME} Box2D)
#target_link_libraries(${PROJECT_NAME} lua)


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



