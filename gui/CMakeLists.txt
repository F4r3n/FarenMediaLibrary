file(GLOB SOURCES "src/*.cpp")

FIND_PACKAGE( PkgConfig REQUIRED )
set(${PROJECT_NAME} FarenEngine)

set(FML_DIR "../")
set(FML_INCLUDE_SOL ${FML_DIR}/extlib/sol2)
set(FML_INCLUDE_STB ${FML_DIR}/extlib/stb)
set(FML_INCLUDE_FREETYPE ${FML_DIR}/extlib/freetype2/include)
set(FML_INCLUDE ${FML_DIR}/lib/include)
set(FML_LIB ${FML_DIR}/build/lib/)
set(FML_INCLUDE_FREETYPE ${FML_DIR}/extlib/freetype2/include)
include_directories(${FML_DIR}/extlib/imgui)
include_directories(../../../ecs/lib/include)
include_directories(${FML_INCLUDE})
include_directories(${FML_INCLUDE_STB})
include_directories(${FML_INCLUDE_SOL})
include_directories(${FML_INCLUDE_FREETYPE})
link_directories(${ENTITY_INCLUDE_DIR}/bin/lib)
include_directories("include")
include_directories("/usr/local/include")
link_directories(${FML_LIB})
link_directories(../../../ecs/bin/lib)



add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} ECS Box2D FarenMediaLibrary )

