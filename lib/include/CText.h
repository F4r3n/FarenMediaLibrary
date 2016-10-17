#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <GL/glew.h>
#include <Component.h>
#include "Color.h"
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Resource.h>
#include "Vector2.h"
#include "RFont.h"
namespace fmc {

class CText : public Component<CText> {
public:
    CText(const std::string& text, const std::string& fontName, unsigned int width, unsigned int height);
    ~CText();
    
    glm::mat4 projection;
    std::string fontName;
    GLuint VAO, VBO;

    std::string text = "Ab";
    GLfloat scale = 1;

private:
    fm::Vector2f pos = { 0, 0 };
};
}