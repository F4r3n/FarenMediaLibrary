cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "FMLEditor")
project(${PROJECT_NAME})


set(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${PROJECT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib/imgui)

set(SOURCES ${SOURCES} 
	src/main.cpp
	src/imgui_impl_sdl3.cpp
	src/imgui_impl_opengl3.cpp
	extlib/imgui/imgui.cpp
	extlib/imgui/imgui_draw.cpp
	extlib/imgui/imgui_widgets.cpp
	extlib/imgui/imgui_tables.cpp
	extlib/imgui/imgui_demo.cpp

	src/inspector/inspector.cpp
	src/inspector/materialInspector.cpp
	src/inspector/meshInspector.cpp
	src/inspector/pointLightInspector.cpp
	src/inspector/scriptManagerInspector.cpp
	src/inspector/transformInspector.cpp
	src/inspector/textInspector.cpp
	src/inspector/bodyInspector.cpp
	src/inspector/colliderInspector.cpp
	src/inspector/cameraInspector.cpp
	src/Editor.cpp
	src/MainWindow.cpp
	src/Window/GWindow.cpp
	src/Window/GEditorView.cpp
	src/Window/GGameView.cpp
	src/Window/GDebugLogger.cpp
	src/Window/GListComponent.cpp
	src/Window/GListEntities.cpp
	src/Window/GToolBar.cpp
	src/Window/GFileNavigator.cpp
	src/Window/GLauncher.cpp
	src/Window/GWindow.cpp
	src/Window/GMaterialEditor.cpp


	src/PickingSystem.cpp
	src/PortableFileDialog.cpp
	src/ImGuizmo/ImGuizmo.cpp
	)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_LIB_PATH ${CMAKE_BINARY_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)

#Bullet
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath BulletSoftBody)
link_directories(${FML_EXTLIB_LIB}/bullet3)
include_directories(${FML_EXTLIB}/bullet3/src)

#Opengl
find_package(OpenGL)


include_directories(${FML_EXTLIB}/openal/include)
link_directories(${FML_EXTLIB_LIB}/openal/lib)


#GLEW
add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)
link_directories(${FML_EXTLIB_LIB}/glew)
include_directories(${FML_EXTLIB}/glew/include)

#SDL3
include_directories(${FML_EXTLIB}/SDL3/include)
link_directories(${FML_EXTLIB_LIB}/SDL3/lib)

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
include_directories(${FML_PATH}/lib/src)
link_directories(${FML_LIB_PATH}/lib)

#IMGUI
include_directories(${FML_EXTLIB}/imgui/)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL3-shared)
target_link_libraries(${PROJECT_NAME} glew_s)
target_link_libraries(${PROJECT_NAME} ECS lua ${BULLET_LIBRARIES} OpenAL)


if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
	set_property( DIRECTORY ${PROJECT_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )
endif(MSVC)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_SOURCE_DIR}/InternalResources $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
					COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_SOURCE_DIR}/InternalResources/imgui.ini $<TARGET_FILE_DIR:${PROJECT_NAME}>)


add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${FML_LIB_PATH}/lib/
                ${CMAKE_CURRENT_BINARY_DIR}/)


