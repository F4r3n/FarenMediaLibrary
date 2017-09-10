#include <ft2build.h>
#include FT_FREETYPE_H
#ifdef __EMSCRIPTEN__
#define USE_GLEW 0
#endif

#ifndef USE_GLEW
#include "GL/glew.h"
#endif


#ifdef USE_GLEW
#include "SDL/SDL_opengl.h"
#endif

#include "Components/CText.h"
#include "Window.h"
using namespace fmc;
const std::string CText::name = "Text renderer";

CText::CText() {}

CText::CText(const std::string& text, const std::string& fontName) {
    this->text = text;
    this->fontName = fontName;
}

CText::~CText() {
}