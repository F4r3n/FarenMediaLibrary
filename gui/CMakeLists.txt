file(GLOB SOURCES "src/*.cpp")


find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
FIND_PACKAGE( PkgConfig REQUIRED )
pkg_search_module( GLFW3 3.3 REQUIRED glfw3 )


set( FML_PATH "../")
set( SOL_INCLUDE_DIR ${FML_PATH}extlib/sol2)
set( STB_INCLUDE_DIR ${FML_PATH}extlib/stb)
set( GLM_INCLUDE_DIR ${FML_PATH}extlib/glm)

set( ENTITY_INCLUDE_DIR ${FML_PATH}extlib/ECS)

set( LUA_INCLUDE_DIR "/usr/local/include")
set( LUA_LIBRARY_DIR "/usr/local/lib")

set(FREETYPE_INCLUDE_DIR ${FML_PATH}extlib/freetype2/include)
set(FREETYPE_LIBRARY_DIR ${FML_PATH}extlib/freetype2/build)


include_directories(${GLFW3_INCLUDE_DIR} )
include_directories(${ENTITY_INCLUDE_DIR}/lib/include )
include_directories(${SOL_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIR})
include_directories(${IMGUI_INCLUDE_DIR})

include_directories(${FML_PATH}/lib/include)

link_directories(${FREETYPE_LIBRARY_DIR} freetype)
link_directories(${ENTITY_INCLUDE_DIR}/build/lib)
link_directories(${FML_PATH}/build/lib)


include_directories(${FML_PATH}/extlib/imgui)

add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARY} lua)
target_link_libraries(${PROJECT_NAME} ${GLFW3_LIBRARIES} Box2D)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJECT_NAME} ECS freetype openal sndfile FarenMediaLibrary)

