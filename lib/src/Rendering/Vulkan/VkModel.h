#pragma once
#include <memory>
#include "vulkan/vulkan.h"
#include <vk_mem_alloc.h>
#include <functional>
namespace fm
{
	class Model;
	class VkModel
	{
	public:
		VkModel(VmaAllocator inAllocator, std::shared_ptr<fm::Model> inModel);
		bool	UploadData(std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit);
		bool	Destroy();
		void	Draw(VkCommandBuffer inCmd, uint32_t inInstanceIndex);
	private:
		std::shared_ptr<Model> _model = nullptr;
		VmaAllocator _allocator = nullptr;
	};
}