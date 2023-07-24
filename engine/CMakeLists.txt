cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME "FMLEngine")
project(${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib/imgui)
message(${CMAKE_CURRENT_LIST_DIR}extlib)
set(SOURCES ${SOURCES} 
	src/main.cpp
	)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
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

#SDL2
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
link_directories(${FML_PATH}/build/lib)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} debug SDL3d optimized SDL3)
target_link_libraries(${PROJECT_NAME} glew_s)
target_link_libraries(${PROJECT_NAME} ECS lua ${BULLET_LIBRARIES} OpenAL)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
endif(MSVC)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_SOURCE_DIR}/InternalResources $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources)


