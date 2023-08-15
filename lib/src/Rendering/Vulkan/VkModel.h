#pragma once
#include <memory>
#include "vulkan/vulkan.h"
#include <vk_mem_alloc.h>
namespace fm
{
	class Model;
	class VkModel
	{
	public:
		VkModel(VmaAllocator inAllocator, std::shared_ptr<fm::Model> inModel);
		bool	UploadData();
		bool	Destroy();
		void	Draw(VkCommandBuffer inCmd);
	private:
		std::shared_ptr<Model> _model = nullptr;
		VmaAllocator _allocator = nullptr;
	};
}