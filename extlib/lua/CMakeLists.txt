cmake_minimum_required (VERSION 2.6)
project (lua)  # project here actually means solution in premake

if(WIN32)
        add_definitions( -D_CRT_SECURE_NO_WARNINGS )

endif()
 add_definitions(-DLUA_COMPAT_5_2)

set(SOURCES 
src/lapi.c 
src/lcode.c 
src/lctype.c 
src/ldebug.c 
src/ldo.c 
src/ldump.c 
src/lfunc.c 
src/lgc.c 
src/llex.c 
src/lmem.c 
src/lobject.c 
src/lopcodes.c 
src/lparser.c 
src/lstate.c 
src/lstring.c 
src/ltable.c 
src/ltm.c 
src/lundump.c 
src/lvm.c 
src/lzio.c 
src/lauxlib.c 
src/lbaselib.c 
src/lbitlib.c 
src/lcorolib.c 
src/ldblib.c 
src/liolib.c 
src/lmathlib.c 
src/loslib.c 
src/lstrlib.c 
src/ltablib.c 
src/loadlib.c 
src/linit.c
src/lutf8lib.c
)
add_library (lualib SHARED ${SOURCES})

#if(FML_BUILD_SHARED_LIBS)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library (lua_shared STATIC ${SOURCES})
set_property(TARGET lua_shared PROPERTY POSITION_INDEPENDENT_CODE ON)
#set_target_properties(lua PROPERTIES OUTPUT_NAME "lua") 
#endif()

