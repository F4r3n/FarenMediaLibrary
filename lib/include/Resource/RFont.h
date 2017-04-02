#pragma once
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <Component.h>
#include "Core/Math/Vector2.h"
#include "Resource/Resource.h"
struct Character {
    fm::math::vec2 advance;
    fm::math::vec2 b_wh;
    fm::math::vec2 b_lt;
   
    fm::math::vec2 t;
    
};
class RFont : public fm::Resource {
public:
    RFont(const std::string& name);
    RFont();
    ~RFont();
    std::unordered_map<GLchar, Character> Characters;
    
    GLuint tex;
    int atlas_width;
    int atlas_height;
    
    
};