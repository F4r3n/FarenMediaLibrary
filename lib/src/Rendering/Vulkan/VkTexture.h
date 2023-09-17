#pragma once
#include <functional>
#include "Rendering/Vulkan/Vulkan.h"
#include <vk_mem_alloc.h>
#include "Core/Math/Vector2.h"
namespace fm
{
	class FilePath;
	class VkTexture {
	public:
		VkTexture(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit);
		bool UploadImage(const fm::FilePath& inPath);
		void Destroy();
		const VkDescriptorImageInfo&	GetDescriptor() const { return _descriptor; }
	private:
		Vulkan* _vulkan = nullptr;
		std::function<void(std::function<void(VkCommandBuffer cmd)>)> _submitBuffer;

		fm::AllocatedImage _allocatedImage;

		VkDescriptorImageInfo _descriptor;
		VkSampler             _sampler;
		VkImageView           _view;
		fm::math::vec2        _size;
		uint32_t              _mipLevels = 0;
		uint32_t              _layerCount;
		VkImageLayout         _imageLayout;
	};
}