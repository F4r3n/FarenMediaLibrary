cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "FarenEngine")
project(${PROJECT_NAME})




include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib/imgui)
message(${CMAKE_CURRENT_LIST_DIR}extlib)
set(SOURCES ${SOURCES} 
	src/main.cpp
	src/imgui_impl_sdl.cpp
	src/imgui_impl_opengl3.cpp
	extlib/imgui/imgui.cpp
	extlib/imgui/imgui_draw.cpp
	extlib/imgui/imgui_widgets.cpp
	extlib/imgui/imgui_demo.cpp

	
	src/MainWindow.cpp
	src/GWindow.cpp
	src/GameView.cpp
	src/SaveProjectWindow.cpp
	src/inspector/inspector.cpp
	src/inspector/materialInspector.cpp
	src/inspector/meshInspector.cpp
	src/inspector/pointLightInspector.cpp
	src/inspector/scriptManagerInspector.cpp
	src/inspector/transformInspector.cpp
	src/inspector/body3DInspector.cpp
	src/debuglogger.cpp
	src/dialogfilebrowser.cpp

	)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Bullet
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath BulletSoftBody)
link_directories(${FML_EXTLIB_LIB}/bullet3)
include_directories(${FML_EXTLIB}/bullet3/src)

#Opengl
find_package(OpenGL)

IF(NOT WIN32)
    SET(OPENAL_LIBRARIES "openal")
ELSE()
    SET(OPENAL_LIBRARIES "OpenAL32")
ENDIF()
include_directories(${FML_EXTLIB}/openal/include)
link_directories(${FML_EXTLIB_LIB}/openal/lib)


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


#FML
include_directories(${FML_PATH}/lib/include)
link_directories(${FML_PATH}/build/lib)

#IMGUI
include_directories(${FML_EXTLIB}/imgui/)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} debug SDL2d optimized SDL2)
target_link_libraries(${PROJECT_NAME} ECS lua ${BULLET_LIBRARIES})


file(COPY ${CMAKE_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



