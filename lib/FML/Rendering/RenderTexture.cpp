#include "RenderTexture.hpp"
#include "Core/FilePath.h"
#include "TextureDef.hpp"
using namespace fm;

RenderTexture::RenderTexture(const fm::FilePath& inPath)
	:Resource(inPath)
{
	_currentID = ++_ID;
}

RenderTexture::RenderTexture(size_t width, size_t height, const std::vector<TextureFormat>& formats,
	const std::vector<TextureType>& types, size_t depth, int multiSampling)
	:Resource(fm::FilePath(std::string("")))
{
	_formats = formats;
	_types = types;
	_width = width;
	_height = height;
	_depth = depth;
	_multiSampling = multiSampling;
	_numberColors = formats.size();
}

void RenderTexture::Load(const nlohmann::json& inJSON)
{
	Resource::Load(inJSON);
}
void RenderTexture::Save(nlohmann::json& outJSON) const
{
	Resource::Save(outJSON);
}

std::shared_ptr<fm::RenderTexture> fm::CreateRenderTexture(size_t width, size_t height, size_t multismapled)
{
	std::vector<fm::TextureFormat> formats{ fm::TextureFormat::RGBA, fm::TextureFormat::RGBA };
	std::vector<fm::TextureType> types{ fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE };
	return std::make_shared<fm::RenderTexture>(width, height, formats, types, 24, multismapled);
}

