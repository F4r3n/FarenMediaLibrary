#pragma once
#include <functional>
#include "Rendering/Vulkan/Vulkan.h"
#include <vk_mem_alloc.h>

namespace fm
{
	class FilePath;
	class VkTexture {
	public:
		VkTexture(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit);
		bool UploadImage(const fm::FilePath& inPath);
		void Destroy();
	private:
		Vulkan* _vulkan = nullptr;
		std::function<void(std::function<void(VkCommandBuffer cmd)>)> _submitBuffer;

		fm::AllocatedImage _allocatedImage;
	};
}