cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "FMLEngine")
project(${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib)


set(SOURCES ${SOURCES} 
	src/main.cpp
	)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)
set(FML_LIB_PATH ${CMAKE_BINARY_DIR})


#Opengl
find_package(OpenGL)

#Vulkan
if(ENABLE_VULKAN)
        find_package(Vulkan REQUIRED)
        include_directories(${Vulkan_INCLUDE_DIRS})
endif(ENABLE_VULKAN)

#SDL3
FETCHCONTENT_GETPROPERTIES(SDL)
include_directories(SYSTEM ${sdl_SOURCE_DIR}/include)
link_directories(${sdl_BINARY_DIR})

#JSON
include_directories(SYSTEM ${FML_EXTLIB}/json/include)

#FML
include_directories(${FML_PATH}/lib/src)
link_directories(${FML_PATH}/build/lib)

#LUA
include_directories(SYSTEM ${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s)

if(ENABLE_PCH)
	target_precompile_headers(${PROJECT_NAME}
  	PRIVATE
		<string>
		<map>
		<unordered_map>
		<filesystem>
)
endif(ENABLE_PCH)


if(ENABLE_VULKAN)
        target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_VULKAN=1)
endif(ENABLE_VULKAN)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
	COMMAND_EXPAND_LISTS
)
endif(MSVC)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/_Resources_ ${CMAKE_SOURCE_DIR}/InternalResources $<TARGET_FILE_DIR:${PROJECT_NAME}>/_Resources_
	COMMAND_EXPAND_LISTS
)
