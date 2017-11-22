#pragma once

#include <unordered_map>

#include <Component.h>
#include "Core/Color.h"

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Resource/RFont.h"
#include "Rendering/VertexBuffer.hpp"
namespace fmc {

class CText : public Component<CText> {
public:
    CText(const std::string& text, const std::string& fontName);
    CText();
    ~CText();
    
        int* get(int v) {return nullptr;}

    std::string fontName = "dejavu";
    std::string text = "";
    std::string previousText = "";
    GLfloat scale = 1;
    
    static const std::string name;

    fm::rendering::VertexBuffer* buffer = nullptr;
        
private:
    fm::math::Vector2f pos = { 0, 0 };
    
    
};
}
