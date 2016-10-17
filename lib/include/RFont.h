#pragma once
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <Component.h>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Resource.h>
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    long Advance;       // Horizontal offset to advance to next glyph
};
class RFont : public fm::Resource {
public:
    RFont(const std::string &name);
    RFont();
    ~RFont();
    std::unordered_map<GLchar, Character> Characters;
    
};