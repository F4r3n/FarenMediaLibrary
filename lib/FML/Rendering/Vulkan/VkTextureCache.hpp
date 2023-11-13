#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "Rendering/Vulkan/Vulkan.hpp"
namespace fm
{
	class VkTexture;
	class Texture;

	struct VkTextureCache
	{
		VkTextureCache(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit);
		std::shared_ptr<fm::VkTexture>	_blankTexture;
		std::unordered_map<uint32_t, std::shared_ptr<fm::VkTexture>> _textures;
		std::shared_ptr<fm::VkTexture> FindOrCreateTexture(std::shared_ptr<fm::Texture> inTexture);
		std::shared_ptr<fm::VkTexture> GetBlank() const { return _blankTexture; }
		void Destroy();

	private:
		Vulkan* _vulkan = nullptr;
		std::function<void(std::function<void(VkCommandBuffer cmd)>)> _submitBuffer;
	};
}
