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
    CText(const std::string& text, const std::string& fontName);
    CText();
    ~CText();
    
    
    std::string fontName = "dejavu";
    GLuint VAO, VBO;

    std::string text = "";
    GLfloat scale = 1;
    
    bool outline = false;
    fm::Color outline_color;
    fm::Vector2f outline_min;
    fm::Vector2f outline_max;
    
    bool soft_edges = false;
    fm::Vector2f soft_edge_values;
    static const std::string name;
    
    #ifdef GUI
    
    char textToRender_GUI[256];
    void display(bool* value) {
        if(ImGui::CollapsingHeader(name.c_str(), value)) {
            ImGui::InputText("Text", textToRender_GUI, 256);
            text = std::string(textToRender_GUI);
           
        }
    }
#endif

private:
    fm::Vector2f pos = { 0, 0 };
};
}