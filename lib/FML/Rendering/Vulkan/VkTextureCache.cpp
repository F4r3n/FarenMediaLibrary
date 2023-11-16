#include "VkTextureCache.hpp"
#include "VkTexture.hpp"
#include "Rendering/Texture.h"
#include "Rendering/Image.h"
#include <cassert>
using namespace fm;

VkTextureCache::VkTextureCache(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit)
{
	_vulkan = inVulkan;
	_submitBuffer = inSubmit;
	if (fm::Image blankImage(true); blankImage.create(fm::math::vec2i(1, 1)))
	{
		_blankTexture = std::make_shared<fm::VkTexture>(_vulkan, _submitBuffer);
		_blankTexture->UploadImage(blankImage);
	}
}

void VkTextureCache::Destroy()
{
	for (const auto& [_, texture] : _textures)
	{
		texture->Destroy();
	}
	_blankTexture->Destroy();
}


std::shared_ptr<fm::VkTexture> VkTextureCache::FindOrCreateTexture(std::shared_ptr<fm::Texture> inTexture)
{
	if (auto it = _textures.find(inTexture->GetObjectID()); it != _textures.end())
	{
		return it->second;
	}

	std::shared_ptr<fm::VkTexture> texture;
	fm::Image image(inTexture->GetPath());
	if (image.LoadImage())
	{
		texture = std::make_shared<fm::VkTexture>(_vulkan, _submitBuffer);
		if (texture->UploadImage(image))
		{
			_textures.emplace(inTexture->GetObjectID(), texture);
		}
	}
	else
	{
		assert(false);
	}

	return texture;
}
