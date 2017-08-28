#pragma once

#include <unordered_map>

#include <Component.h>
#include "Core/Color.h"

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Resource/RFont.h"
#include "Serializer.h"
namespace fmc {

class CText : public Component<CText>, public Serializer {
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
    fm::math::Vector2f outline_min;
    fm::math::Vector2f outline_max;
    
    bool soft_edges = false;
    fm::math::Vector2f soft_edge_values;
    static const std::string name;
void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override{}
    void parse(rapidjson::Value &value) override {}
private:
    fm::math::Vector2f pos = { 0, 0 };
};
}