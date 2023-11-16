#include "OGLTextureCache.hpp"
#include "OGLTexture.hpp"
#include "Rendering/Texture.h"
#include "Rendering/Image.h"
#include <cassert>
using namespace fm;

std::shared_ptr<fm::OGLTexture> OGLTextureCache::FindOrCreateTexture(std::shared_ptr<fm::Texture> inTexture)
{
	if (auto it = _textures.find(inTexture->GetObjectID()); it != _textures.end())
	{
		return it->second;
	}

	std::shared_ptr<fm::OGLTexture> texture;
	fm::Image image(inTexture->GetPath());
	if (image.LoadImage())
	{
		texture = std::make_shared<fm::OGLTexture>(image);
		_textures.emplace(inTexture->GetObjectID(), texture);
	}
	else
	{
		assert(false);
	}

	inTexture->SetDestroyCallback(std::bind(&OGLTextureCache::Release, this, std::placeholders::_1));


	return texture;
}

void OGLTextureCache::Release(uint32_t inID)
{
	if (auto it = _textures.find(inID); it != _textures.end())
	{
		_textures.erase(it);
	}
}

