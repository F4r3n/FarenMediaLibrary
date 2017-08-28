project (glew)

cmake_minimum_required (VERSION 2.8.12)

include(GNUInstallDirs)

if (COMMAND cmake_policy)
  cmake_policy (SET CMP0003 NEW)
  cmake_policy (SET CMP0042 NEW)
endif()

#set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package (OpenGL REQUIRED)

if (WIN32)
  set (GLEW_LIB_NAME glew32)
else ()
  set (GLEW_LIB_NAME GLEW)
  set (DLL_PREFIX lib)
endif ()

set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})

add_definitions (-DGLEW_NO_GLU)

#### GLEW ####
set (GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${GLEW_DIR}/include)

set (GLEW_PUBLIC_HEADERS_FILES ${GLEW_DIR}/include/GL/wglew.h ${GLEW_DIR}/include/GL/glew.h ${GLEW_DIR}/include/GL/glxew.h)
set (GLEW_SRC_FILES ${GLEW_DIR}/src/glew.c)

if (WIN32)
  list (APPEND GLEW_SRC_FILES ${GLEW_DIR}/build/glew.rc)
endif ()

add_library (glew SHARED ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})

add_library (glew_s STATIC ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})
set_target_properties (glew_s PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX lib)
