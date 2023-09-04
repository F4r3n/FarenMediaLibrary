cmake_minimum_required(VERSION 3.15)
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
if(NOT WANT_VULKAN)
  LIST(FILTER SOURCES EXCLUDE REGEX "Vulkan/")
  LIST(FILTER HEADERS EXCLUDE REGEX "Vulkan/")
endif(WANT_VULKAN)

#Bullet
set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath BulletSoftBody)
FETCHCONTENT_GETPROPERTIES(BULLET)
include_directories(${bullet_SOURCE_DIR}/src)
link_directories(${bullet_BINARY_DIR})

#Opengl
find_package(OpenGL)

#OpenAL
FETCHCONTENT_GETPROPERTIES(OPENAL)
include_directories(${openal_SOURCE_DIR}/lib/include)
link_directories(${openal_BINARY_DIR})


#GLEW
add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)
include_directories(${FML_EXTLIB}/glew/include)
link_directories(${FML_EXTLIB_LIB}/glew)

if(WANT_VULKAN)
  #Vulkan
  find_package(Vulkan REQUIRED FATAL_ERROR OPTIONAL_COMPONENTS shaderc_combined)
  include_directories(${Vulkan_INCLUDE_DIRS})

  #VMA
  FETCHCONTENT_GETPROPERTIES(VMA)
  include_directories(${vma_SOURCE_DIR}/include)
endif(WANT_VULKAN)

#SDL3
FETCHCONTENT_GETPROPERTIES(SDL)
include_directories(${sdl_SOURCE_DIR}/include)
link_directories(${sdl_BINARY_DIR})

#sol2
include_directories(${FML_EXTLIB}/sol2)

#STB
include_directories(${FML_EXTLIB}/stb)

#RAPID_JSON
include_directories(${FML_EXTLIB}/json/include)


#ECS
FETCHCONTENT_GETPROPERTIES(ECS)
include_directories(${ecs_SOURCE_DIR}/lib/include) 
link_directories(${ecs_BINARY_DIR}/lib)

#FREETYPE
include_directories(${FML_EXTLIB}/freetype2/include)
link_directories(${FML_EXTLIB_LIB}/freetype2)

#LUA
include_directories(${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

#Assimp
FETCHCONTENT_GETPROPERTIES(ASSIMP)
include_directories(${assimp_SOURCE_DIR}/include) 
link_directories(${assimp_BINARY_DIR}/code)


set(LIB_NAME ${PROJECT_NAME}_s)

add_library(${LIB_NAME} STATIC ${SOURCES} ${HEADERS})
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
    
target_link_libraries(${LIB_NAME} freetype)
target_link_libraries(${LIB_NAME} ECS)
target_link_libraries(${LIB_NAME} lua)
target_link_libraries(${LIB_NAME} SDL3-shared)
target_link_libraries(${LIB_NAME} glew_s)
target_link_libraries(${LIB_NAME} ${OPENGL_LIBRARIES} )
target_link_libraries(${LIB_NAME} OpenAL)
target_link_libraries(${LIB_NAME} assimp)
target_link_libraries(${LIB_NAME} ${BULLET_LIBRARIES})
target_link_libraries(${LIB_NAME} ${CMAKE_DL_LIBS})
if(WANT_VULKAN)
  target_link_libraries(${LIB_NAME} Vulkan::Vulkan)
  target_link_libraries(${LIB_NAME} Vulkan::shaderc_combined)
  target_compile_definitions(${LIB_NAME} WITH_VULKAN=1)
endif(WANT_VULKAN)

if(MSVC)
  target_compile_options(${LIB_NAME} PRIVATE /W4)
else()
  target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(${LIB_NAME} gcc_s stdc++) 
	target_link_libraries(${LIB_NAME} stdc++fs)
endif(CMAKE_COMPILER_IS_GNUCXX)