#pragma once

#define PRINT(x) std::cout << #x << std::endl;

#define PHYSIC_SYSTEM
#define SCRIPT_SYSTEM 

#define ANDROID 0
#define WEBGL 0
#define DESKTOP 1
#define OPENGL_CORE 0
#define OPENGL_ES 1

#if ANDROID == 1
#undef OPENGL_CORE
#undef OPENGL_ES
#define OPENGL_ES 1
#endif
