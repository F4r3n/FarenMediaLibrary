#pragma once

#include "component.h"
#include "Core/Math/Vector2.h"


namespace fm
{
namespace rendering
{
class VertexBuffer;
}
}

class EntityManager;
namespace fmc {

class CText : public FMComponent<CText> {
    public:
        CText(const std::string& text, const std::string& fontName);
        CText();
        ~CText();

        std::string fontName = "dejavu";
        std::string text = "";
        std::string previousText = "";
        float scale = 1;

        static const std::string name;

        fm::rendering::VertexBuffer* buffer = nullptr;

        void Destroy() override;
        
    private:
        fm::math::Vector2f pos = { 0, 0 };


};
}