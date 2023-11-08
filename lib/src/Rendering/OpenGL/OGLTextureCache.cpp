#include "OGLTextureCache.hpp"
#include "OGLTexture.hpp"
#include "Rendering/Texture.h"
#include "Rendering/Image.h"
#include <cassert>
using namespace fm;

std::shared_ptr<fm::OGLTexture> TextureCache::FindOrCreateTexture(std::shared_ptr<fm::Texture> inTexture)
{
	if (auto it = _textures.find(inTexture->GetID()); it != _textures.end())
	{
		return it->second;
	}

	std::shared_ptr<fm::OGLTexture> texture;
	fm::Image image(inTexture->GetPath());
	if (image.LoadImage())
	{
		texture = std::make_shared<fm::OGLTexture>();
		_textures.emplace(inTexture->GetID(), texture);
	}
	else
	{
		assert(false);
	}

	return texture;
}
