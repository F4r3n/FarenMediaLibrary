
#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
#include "Core/FilePath.h"


using namespace fm;

Shader::Shader():Resource(fm::FilePath(std::string(""))) {
}

Shader::Shader(const fm::FilePath& inFilePath, const std::string& name) : Resource(inFilePath)
{
    _name = name;
}


Shader::~Shader() {
}


