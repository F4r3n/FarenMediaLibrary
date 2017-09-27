#pragma once

#define PRINT(x) std::cout << #x << std::endl;




#define PHYSIC_SYSTEM
#define SCRIPT_SYSTEM 

#define ANDROID 0
#define WEBGL 0
#define DESKTOP 1
#define OPENGL_CORE 0
#define OPENGL_ES 1
#if OPENGL_ES == 1
#define OPENGL_ES_VERSION 3
#endif

#if ANDROID == 1
#undef OPENGL_CORE
#undef OPENGL_ES
#define OPENGL_ES 1
#endif

#ifdef __EMSCRIPTEN__
#define USE_GLEW 0
#else 
#define USE_GLEW 1
#endif

#if USE_GLEW
#include "GL/glew.h"
//#include "SDL2/SDL_opengles2.h"
#endif

#if !USE_GLEW
#if OPENGL_ES_VERSION == 2
#include "SDL2/SDL_opengles2.h"
#endif
#include "SDL/SDL_opengl.h"
#endif
