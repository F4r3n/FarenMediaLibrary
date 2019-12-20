project (glew C)

cmake_minimum_required (VERSION 2.8.12)

if (COMMAND cmake_policy)
  cmake_policy (SET CMP0003 NEW)
  cmake_policy (SET CMP0042 NEW)
endif()


set (GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR})


set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})

add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)

include_directories (${GLEW_DIR}/include)

set (GLEW_PUBLIC_HEADERS_FILES ${GLEW_DIR}/include/GL/wglew.h ${GLEW_DIR}/include/GL/glew.h ${GLEW_DIR}/include/GL/glxew.h)
set (GLEW_SRC_FILES ${GLEW_DIR}/src/glew.c)


add_library (glew_s STATIC ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})
