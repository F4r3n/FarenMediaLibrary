cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "Tests")
project(${PROJECT_NAME})


set(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${PROJECT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/extlib)

set(SOURCES ${SOURCES} 
	src/test_main.cpp
)

include_directories("src")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

set(FML_PATH ${CMAKE_SOURCE_DIR})
set(FML_LIB_PATH ${CMAKE_BINARY_DIR})
set(FML_EXTLIB ${FML_PATH}/extlib) 
set(FML_EXTLIB_LIB ${CMAKE_BINARY_DIR}/extlib)

#GLEW
add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)
include_directories(${FML_EXTLIB}/glew/include)

#sol2
include_directories(SYSTEM ${FML_EXTLIB}/sol2)

#JSON
include_directories(SYSTEM ${FML_EXTLIB}/json/include)

#ECS
FETCHCONTENT_GETPROPERTIES(ECS)
include_directories(SYSTEM ${ecs_SOURCE_DIR}/lib/include)

#FML
include_directories(${FML_PATH}/lib/src)
link_directories(${FML_LIB_PATH}/lib)

#LUA
include_directories(SYSTEM ${FML_EXTLIB}/lua/src)
link_directories(${FML_EXTLIB_LIB}/lua)


#GLSLANG
FETCHCONTENT_GETPROPERTIES(GLSLANG)
include_directories(${glslang_SOURCE_DIR})
link_directories(${glslang_BINARY_DIR})

FETCHCONTENT_GETPROPERTIES(DOCTEST)
include_directories(SYSTEM ${doctest_SOURCE_DIR})


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} FarenMediaLibrary_s)


if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
	set_property( DIRECTORY ${PROJECT_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )
endif(MSVC)


add_test(
        NAME ${PROJECT_NAME}
        COMMAND ${PROJECT_NAME})

add_test(NAME "All"
  COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)
