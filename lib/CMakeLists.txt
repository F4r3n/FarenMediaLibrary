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

file(GLOB_RECURSE SOURCES FML/*.cpp)

file(GLOB_RECURSE HEADERS "FML/*.h" "FML/*.hpp")
if(NOT ENABLE_VULKAN)
  LIST(FILTER SOURCES EXCLUDE REGEX "Vulkan/")
  LIST(FILTER HEADERS EXCLUDE REGEX "Vulkan/")
endif(NOT ENABLE_VULKAN)

#Opengl
find_package(OpenGL)




set(LIB_NAME ${PROJECT_NAME}_s)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${LIB_NAME} PUBLIC "FML")
#sol2
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/sol2)
#STB
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/stb)

#LUA
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/lua/src)
target_link_directories(${LIB_NAME} PRIVATE ${FML_EXTLIB_LIB}/lua)

#Bullet
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath BulletSoftBody)
FETCHCONTENT_GETPROPERTIES(BULLET)
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${bullet_SOURCE_DIR}/src)
target_link_directories(${LIB_NAME} PRIVATE ${bullet_BINARY_DIR})


#GLEW
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/glew/include)
target_link_directories(${LIB_NAME} PRIVATE ${FML_EXTLIB_LIB}/glew)

if(ENABLE_VULKAN)
  #Vulkan
  find_package(Vulkan REQUIRED)
  target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})

  #VMA
  FETCHCONTENT_GETPROPERTIES(VMA)
  target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${vma_SOURCE_DIR}/include)

endif(ENABLE_VULKAN)

#SDL3
FETCHCONTENT_GETPROPERTIES(SDL)
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${sdl_SOURCE_DIR}/include)
target_link_directories(${LIB_NAME} PRIVATE ${sdl_BINARY_DIR})

#JSON
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/json/include)


#ENTT
FETCHCONTENT_GETPROPERTIES(ENTT)
target_include_directories(${LIB_NAME} PRIVATE ${entt_SOURCE_DIR}/src) 
target_link_directories(${LIB_NAME} PRIVATE ${entt_BINARY_DIR}/lib)

#FREETYPE
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${FML_EXTLIB}/freetype2/include)
target_link_directories(${LIB_NAME} PRIVATE ${FML_EXTLIB_LIB}/freetype2)

#Assimp
if(WANT_ASSIMP)
	FETCHCONTENT_GETPROPERTIES(ASSIMP)
	target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${assimp_SOURCE_DIR}/include) 
	target_link_directories(${LIB_NAME} PRIVATE ${assimp_BINARY_DIR}/code)
else()
	FETCHCONTENT_GETPROPERTIES(TINYOBJLOADER)
	target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${tinyobjloader_SOURCE_DIR}) 
	target_link_directories(${LIB_NAME} PRIVATE ${tinyobjloader_BINARY_DIR}/lib)
endif()



set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
    
target_link_libraries(${LIB_NAME} freetype)
target_link_libraries(${LIB_NAME} lua)
target_link_libraries(${LIB_NAME} SDL3-static)
target_link_libraries(${LIB_NAME} glew_s)
target_link_libraries(${LIB_NAME} OpenGL::GL )
target_link_libraries(${LIB_NAME} EnTT)

if(ENABLE_PCH)
  target_precompile_headers(${LIB_NAME}
    PRIVATE
  	<string>
  	<map>
  	<filesystem>
  	<unordered_map>
  )
endif(ENABLE_PCH)

target_precompile_headers(${LIB_NAME}
  PRIVATE
	<string>
    <filesystem>
	<map>
	<unordered_map>
	<nlohmann/json.hpp>
)

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