cmake_minimum_required (VERSION 2.6)
project (lua)  # project here actually means solution in premake

if(WIN32)
        add_definitions( -D_CRT_SECURE_NO_WARNINGS )

endif()
 add_definitions(-DLUA_COMPAT_5_2)

set(SOURCES 
lapi.c      lcorolib.c  ldump.c   llex.c      lopcodes.c  lstrlib.c 
lauxlib.c   lctype.c    lfunc.c   lmathlib.c  loslib.c    ltable.c   lundump.c
lbaselib.c  ldblib.c    lgc.c     lmem.c      lparser.c   ltablib.c  lutf8lib.c
lbitlib.c   ldebug.c    linit.c   loadlib.c   lstate.c    ltm.c      lvm.c
lcode.c     ldo.c       liolib.c  lobject.c   lstring.c        lzio.c

)
#add_library (lualib STATIC ${SOURCES})

#if(FML_BUILD_SHARED_LIBS)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library (lua_shared SHARED ${SOURCES})
set_property(TARGET lua_shared PROPERTY POSITION_INDEPENDENT_CODE ON)
#set_target_properties(lua PROPERTIES OUTPUT_NAME "lua") 
#endif()


# 2. lua interpreter
link_directories (${LUA_BINARY_DIR})
add_executable (lua lua.c)
target_link_libraries (lua lua_shared)
if(UNIX)
        target_link_libraries( lua m )
endif()

# 3. lua compiler
link_directories (${LUA_BINARY_DIR})
add_executable (luac luac.c)
target_link_libraries (luac lua_shared)
if(UNIX)
        target_link_libraries( luac m )
endif()

