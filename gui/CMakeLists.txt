cmake_minimum_required(VERSION 3.16)
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
	extlib/imgui/imgui_user.cpp
	extlib/imgui/misc/cpp/imgui_stdlib.cpp

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
	src/Window/GTextureEditor.cpp
	src/Export.cpp
	src/ShaderCompiler.cpp
	src/PickingSystem.cpp
	src/PortableFileDialog.cpp
	src/ImGuizmo/ImGuizmo.cpp
	src/Resources/IconManager.cpp
	)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_LIB_PATH ${CMAKE_BINARY_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)




add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

#GLEW
add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)
target_include_directories(${PROJECT_NAME} PRIVATE ${FML_EXTLIB}/glew/include)

#sol2
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/sol2)

#JSON
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/json/include)

#ECS
FETCHCONTENT_GETPROPERTIES(ECS)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${ecs_SOURCE_DIR}/lib/include)

#FML
target_include_directories(${PROJECT_NAME} PRIVATE ${FML_PATH}/lib)
target_link_directories(${PROJECT_NAME} PRIVATE ${FML_LIB_PATH}/lib)

#LUA
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/lua/src)
target_link_directories(${PROJECT_NAME} PRIVATE ${FML_EXTLIB_LIB}/lua)

#IMGUI
target_include_directories(${PROJECT_NAME} PRIVATE ${FML_EXTLIB}/imgui/)

#GLSLANG
FETCHCONTENT_GETPROPERTIES(GLSLANG)
target_include_directories(${PROJECT_NAME} PRIVATE ${glslang_SOURCE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${glslang_BINARY_DIR})


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s)
target_link_libraries(${PROJECT_NAME} glslang SPIRV glslang-default-resource-limits)


if(ENABLE_PCH)
	target_precompile_headers(${PROJECT_NAME}
  	PRIVATE
		<string>
		<map>
		<unordered_map>
		<filesystem>
)
endif(ENABLE_PCH)



if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
if(ENABLE_PROFILER)
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
endif(ENABLE_PROFILER)
	set_property( DIRECTORY ${PROJECT_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
	COMMAND_EXPAND_LISTS
)
endif(MSVC)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/_Resources_ ${CMAKE_SOURCE_DIR}/InternalResources $<TARGET_FILE_DIR:${PROJECT_NAME}>/_Resources_ 	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/InternalResources/imgui.ini $<TARGET_FILE_DIR:${PROJECT_NAME}>
	COMMAND_EXPAND_LISTS
)
