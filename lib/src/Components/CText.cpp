#include "Components/CText.h"
#include "Window.h"
using namespace fmc;
const std::string CText::name = "Text renderer";

CText::CText() {}

CText::CText(const std::string& text, const std::string& fontName) {
    this->text = text;
    this->fontName = fontName;
}

CText::~CText() {
}