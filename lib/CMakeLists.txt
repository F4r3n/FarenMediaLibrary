cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "FarenMediaLibrary")
project(${PROJECT_NAME} CXX)
include(CheckCXXCompilerFlag)
SET_COMPILER_OPTIONS()

set(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${PROJECT_BINARY_DIR})

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)
option(FML_BUILD_STATIC ON)
option(FML_BUILD_SHARED OFF)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")
if(NOT ENABLE_VULKAN)
  LIST(FILTER SOURCES EXCLUDE REGEX "Vulkan/")
  LIST(FILTER HEADERS EXCLUDE REGEX "Vulkan/")
endif(NOT ENABLE_VULKAN)

#Bullet
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath BulletSoftBody)
FETCHCONTENT_GETPROPERTIES(BULLET)
include_directories(SYSTEM ${bullet_SOURCE_DIR}/src)
link_directories(${bullet_BINARY_DIR})

#Opengl
find_package(OpenGL)


#GLEW
include_directories(${FML_EXTLIB}/glew/include)
link_directories(${FML_EXTLIB_LIB}/glew)

if(ENABLE_VULKAN)
  #Vulkan
  find_package(Vulkan REQUIRED)
  include_directories(SYSTEM ${Vulkan_INCLUDE_DIRS})

  #VMA
  FETCHCONTENT_GETPROPERTIES(VMA)
  include_directories(SYSTEM ${vma_SOURCE_DIR}/include)

endif(ENABLE_VULKAN)

#SDL3
FETCHCONTENT_GETPROPERTIES(SDL)
include_directories(SYSTEM ${sdl_SOURCE_DIR}/include)
link_directories(${sdl_BINARY_DIR})

#sol2
include_directories(SYSTEM ${FML_EXTLIB}/sol2)

#STB
include_directories(SYSTEM ${FML_EXTLIB}/stb)

#JSON
include_directories(SYSTEM ${FML_EXTLIB}/json/include)


#ECS
FETCHCONTENT_GETPROPERTIES(ECS)
include_directories(${ecs_SOURCE_DIR}/lib/include) 
link_directories(${ecs_BINARY_DIR}/lib)

#FREETYPE
include_directories(SYSTEM ${FML_EXTLIB}/freetype2/include)
link_directories(${FML_EXTLIB_LIB}/freetype2)

#LUA
include_directories(SYSTEM ${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

#Assimp
if(WANT_ASSIMP)
	FETCHCONTENT_GETPROPERTIES(ASSIMP)
	include_directories(SYSTEM ${assimp_SOURCE_DIR}/include) 
	link_directories(${assimp_BINARY_DIR}/code)
else()
	FETCHCONTENT_GETPROPERTIES(TINYOBJLOADER)
	include_directories(SYSTEM ${tinyobjloader_SOURCE_DIR}) 
	link_directories(${tinyobjloader_BINARY_DIR}/lib)
endif()




set(LIB_NAME ${PROJECT_NAME}_s)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
    
target_link_libraries(${LIB_NAME} freetype)
target_link_libraries(${LIB_NAME} ECS)
target_link_libraries(${LIB_NAME} lua)
target_link_libraries(${LIB_NAME} SDL3-static)
target_link_libraries(${LIB_NAME} glew_s)
target_link_libraries(${LIB_NAME} OpenGL::GL )
if(ENABLE_PCH)
  target_precompile_headers(${LIB_NAME}
    PRIVATE
  	<string>
  	<map>
  	<filesystem>
  	<unordered_map>
  )
endif(ENABLE_PCH)

if(ENABLE_ASSIMP)
	target_link_libraries(${LIB_NAME} assimp)
	target_compile_definitions(${LIB_NAME} PRIVATE WITH_ASSIMP=1)
else()
	target_link_libraries(${LIB_NAME} tinyobjloader)
endif(ENABLE_ASSIMP)

target_link_libraries(${LIB_NAME} ${BULLET_LIBRARIES})
target_link_libraries(${LIB_NAME} ${CMAKE_DL_LIBS})
if(ENABLE_VULKAN)
  target_link_libraries(${LIB_NAME} Vulkan::Vulkan)
  target_compile_definitions(${LIB_NAME} PRIVATE WITH_VULKAN=1)
endif(ENABLE_VULKAN)

target_compile_definitions (${LIB_NAME} PRIVATE GLEW_STATIC=1 )
target_compile_definitions (${LIB_NAME} PRIVATE GLEW_NO_GLU=1 )

if(MSVC)
  target_compile_options(${LIB_NAME} PRIVATE /W4)
else()
  target_compile_options(${LIB_NAME} PRIVATE -Wall -Wpedantic)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(${LIB_NAME} gcc_s stdc++) 
	target_link_libraries(${LIB_NAME} stdc++fs)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(ENABLE_COMPILER_PROFILER)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${LIB_NAME} PRIVATE -ftime-trace)
	endif()
endif(ENABLE_COMPILER_PROFILER)

if(ENABLE_PROFILER)
  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
  endif()
endif(ENABLE_PROFILER)