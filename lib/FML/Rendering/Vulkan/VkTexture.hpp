#pragma once
#include <functional>
#include "Rendering/Vulkan/Vulkan.hpp"
#include <vk_mem_alloc.h>
#include "Core/Math/Vector2.h"
namespace fm
{
	class FilePath;
	class Image;
	class VkTexture {
	public:
		VkTexture(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>& inSubmit);
		bool UploadImage(const fm::FilePath& inPath);
		bool UploadImage(const fm::Image &inImage);
		void Destroy();
		const VkDescriptorImageInfo&	GetDescriptor() const { return _descriptor; }
	private:

		bool _UploadImage(const fm::Image& inImage);

		Vulkan* _vulkan = nullptr;
		std::function<void(std::function<void(VkCommandBuffer cmd)>)> _submitBuffer;

		fm::AllocatedImage _allocatedImage;

		VkDescriptorImageInfo _descriptor;
		VkSampler             _sampler;
		VkImageView           _view;
		fm::math::vec2i       _size;
		uint32_t              _mipLevels = 0;
		uint32_t              _layerCount;
		VkImageLayout         _imageLayout;
	};
}