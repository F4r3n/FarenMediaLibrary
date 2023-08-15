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
		uint32_t GetID() const { return _ID; }
		bool	UploadData();
		bool	Destroy();
		void	Draw(VkCommandBuffer inCmd);
	private:
		std::shared_ptr<Model> _model = nullptr;
		inline static uint32_t _ID = 0;
		VmaAllocator _allocator = nullptr;
	};
}