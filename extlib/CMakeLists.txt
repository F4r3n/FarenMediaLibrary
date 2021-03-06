
set(BUILD_SHARED_LIBS OFF)

#ECS
set(ECS_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(ECS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ECS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ECS_BUILD_TESTS OFF CACHE BOOL "" FORCE)

#ASSIMP
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(EXTLIB_CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/extlib)

#SDL
set(SDL_SHARED_ENABLED_BY_DEFAULT ON)
set(SDL_SHARED ON)
set(SDL_STATIC OFF)

#OpenAL
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_TESTS OFF)
set(ALSOFT_UTILS OFF)

#Bullet
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON)

#freetype2
set(SKIP_INSTALL_ALL ON)
set(FREETYPE_NO_DIST ON)

foreach(TARGET freetype2 SDL2 ECS lua assimp openal glew bullet3)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EXTLIB_CMAKE_BINARY_DIR}/${TARGET}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXTLIB_CMAKE_BINARY_DIR}/${TARGET}/lib)

message(${TARGET})
#Assimp is too big with a Static lib
	if(${TARGET} STREQUAL "assimp")
	set(BUILD_SHARED_LIBS ON)
	endif()
	add_subdirectory(${TARGET})
    set(BUILD_SHARED_LIBS OFF)

endforeach()

