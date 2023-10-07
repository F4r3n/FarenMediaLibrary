
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

void Shader::Save(nlohmann::json& outJSON) const
{
	fm::Resource::Save(outJSON);
}

void Shader::Load(const nlohmann::json& inJSON)
{
	fm::Resource::Load(inJSON);
}



Shader::~Shader()
{
}


