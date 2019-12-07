#pragma once

#define PRINT(x) std::cout << #x << std::endl;


#define PHYSIC_SYSTEM
#define SCRIPT_SYSTEM 

#define ANDROID 0
#define WEBGL 0
#define DESKTOP 1
#define OPENGL_CORE 1
#define OPENGL_ES 0


#ifdef __EMSCRIPTEN__
    #undef OPENGL_ES
    #define OPENGL_ES 1
    #undef OPENGL_CORE
#endif

#if OPENGL_ES == 1
    #define OPENGL_ES_VERSION 3
#endif

#if ANDROID == 1
    #undef OPENGL_CORE
    #undef OPENGL_ES
    #define OPENGL_ES 1
#endif

#if OPENGL_ES == 1
    #define USE_GLEW 0
#else 
    #define USE_GLEW 1
#endif

#if USE_GLEW
    #include "GL/glew.h"
#endif

#if !USE_GLEW
    #if OPENGL_ES_VERSION == 3
        #include <GLES3/gl3.h>
    #endif
#endif
