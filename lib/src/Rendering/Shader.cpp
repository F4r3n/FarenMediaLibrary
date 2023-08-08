
#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
using namespace fm;

Shader::Shader():Resource(fm::FilePath("")) {
}

Shader::Shader(const fm::FilePath& inFilePath, const std::string& name) : Resource(inFilePath)
{
    _name = name;
}


Shader::~Shader() {
}
