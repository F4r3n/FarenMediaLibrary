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
    glm::vec2 advance;
    glm::vec2 b_wh;
    glm::vec2 b_lt;
   
    glm::vec2 t;
    
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