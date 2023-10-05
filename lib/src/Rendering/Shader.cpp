
#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
#include "Core/FilePath.h"


using namespace fm;

Shader::Shader():Resource(fm::FilePath(std::string(""))) {
}

Shader::Shader(const fm::FilePath& inFilePath) : Resource(inFilePath)
{
    _name = inFilePath.GetName(true);
}


Shader::~Shader() {
}


