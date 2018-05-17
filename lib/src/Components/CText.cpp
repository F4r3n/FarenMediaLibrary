#include "Components/CText.h"
#include "Window.h"
using namespace fmc;
const std::string CText::name = "Text renderer";

CText::CText()
{
     _name = "Text";
}

CText::CText(const std::string& text, const std::string& fontName)
{
    this->text = text;
    this->fontName = fontName;
    _name = "Text";
}

CText::~CText() {
}
