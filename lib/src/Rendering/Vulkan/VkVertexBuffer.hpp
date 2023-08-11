#pragma once
#include <Core/Math/Vector2.h>
#include "vulkan/vulkan.h"
#include <array>
#include <vk_mem_alloc.h>
#include "Rendering/Mesh.hpp"
#include "Rendering/VertexBuffer.hpp"

namespace fm
{

	struct AllocatedBuffer {
		VkBuffer _buffer;
		VmaAllocation _allocation;
	};


		class VkVertexBuffer : public rendering::VertexBuffer
		{
		public:

			VkVertexBuffer(VmaAllocator inAllocator);
			VkVertexBuffer() = default;
			~VkVertexBuffer();

			//TODO refactor
			virtual void generate(const std::vector<rendering::Vertex>& vertices);
			virtual void destroy();

			static VkVertexInputBindingDescription GetBindingDescription();
			static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
		private:
			VmaAllocator _allocator;
		public:
			AllocatedBuffer	_allocatedBuffer;
		};
	
}
