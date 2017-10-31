#pragma once

#include <unordered_map>

#include <Component.h>
#include "Core/Color.h"

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Resource/RFont.h"
#include "Serializer.h"
#include "Rendering/VertexBuffer.hpp"
namespace fmc {

class CText : public Component<CText>, public Serializer {
public:
    CText(const std::string& text, const std::string& fontName);
    CText();
    ~CText();
    
    
    std::string fontName = "dejavu";
    std::string text = "";
    std::string previousText = "";
    GLfloat scale = 1;
    
    static const std::string name;

    fm::rendering::VertexBuffer* buffer = nullptr;
        
    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override{}
    void parse(rapidjson::Value &value) override {}
    

private:
    fm::math::Vector2f pos = { 0, 0 };
    
    
};
}