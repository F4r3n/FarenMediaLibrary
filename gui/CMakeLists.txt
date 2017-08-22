file(GLOB SOURCES "src/*.cpp")


include_directories(include)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua 5.3 REQUIRED)
find_package(Box2D REQUIRED)
find_package(SDL2 REQUIRED sdl2)
if(UNIX)
FIND_PACKAGE( PkgConfig REQUIRED )
pkg_search_module( GLFW3 3.3 REQUIRED glfw3 )


endif(UNIX)
if(WIN32)

find_package(GLFW3 3.3 REQUIRED glfw3)
endif(WIN32)

set( FML_PATH "../")
set( SOL_INCLUDE_DIR ${FML_PATH}extlib/sol2)
set( STB_INCLUDE_DIR ${FML_PATH}extlib/stb)
set( GLM_INCLUDE_DIR ${FML_PATH}extlib/glm)
set( JSON_INCLUDE_DIR ${FML_PATH}extlib/rapidjson/include)
set( ENTITY_INCLUDE_DIR ${FML_PATH}extlib/ECS)


set(FREETYPE_INCLUDE_DIR ${FML_PATH}extlib/freetype2/include)
set(FREETYPE_LIBRARY_DIR ${FML_PATH}extlib/freetype2/build)

include_directories(${JSON_INCLUDE_DIR})
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
include_directories(include)
link_directories(${FREETYPE_LIBRARY_DIR} freetype)
link_directories(${ENTITY_INCLUDE_DIR}/build/lib)
link_directories(${FML_PATH}/build/lib)


include_directories(${FML_PATH}/extlib/imgui)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES} 
	${GLFW3_LIBRARIES} ${BOX2D_LIBRARIES}  ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES}
	${GLEW_LIBRARY} ECS freetype openal sndfile FarenMediaLibrary)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)


