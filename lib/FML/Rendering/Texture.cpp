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
	_mipmapLevel = inJSON["params"]["mipmapLevel"];
	_canalNumber = inJSON["params"]["canalNumber"];
}

void Texture::Save(nlohmann::json& outJSON) const
{
	Resource::Save(outJSON);

	outJSON["params"]["mipmapLevel"] = _mipmapLevel;
	outJSON["params"]["canalNumber"] = _canalNumber;
}
