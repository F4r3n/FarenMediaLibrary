#include "Texture.h"
#include "nlohmann/json.hpp"
using namespace fm;

Texture::Texture(const fm::FilePath& inPath)
	:Resource(inPath)
{
	_currentID = ++_ID;
}

void Texture::Load(const nlohmann::json& inJSON)
{
	Resource::Load(inJSON);
}

void Texture::Save(nlohmann::json& outJSON) const
{
	Resource::Save(outJSON);
}

