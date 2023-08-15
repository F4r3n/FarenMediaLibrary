#pragma once
#include <Core/Math/Vector2.h>
#include "vulkan/vulkan.h"
#include <array>
#include <vk_mem_alloc.h>
#include "Rendering/Mesh.hpp"
#include "Rendering/VertexBuffer.hpp"

namespace fm
{
	namespace rendering
	{
		struct MeshContainer;
	}
	struct AllocatedBuffer {
		VkBuffer		_buffer		= nullptr;
		VmaAllocation	_allocation = nullptr;
	};


		class VkVertexBuffer : public rendering::VertexBuffer
		{
		public:

			VkVertexBuffer(VmaAllocator inAllocator);
			VkVertexBuffer() = default;
			~VkVertexBuffer();

			//TODO refactor
			virtual void UploadData(const fm::rendering::MeshContainer& inMeshContainer);
			virtual void destroy();
			virtual bool isGenerated() const { return _allocatedBuffer._buffer != nullptr; }
			static VkVertexInputBindingDescription GetBindingDescription();
			static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
		private:
			template <typename T>
			bool		_SetupBuffer(AllocatedBuffer& buffer, const std::vector<T>& data, int TYPE);

			VmaAllocator _allocator = nullptr;
		public:
			AllocatedBuffer	_allocatedBuffer;
			AllocatedBuffer	_allocatedIndexBuffer;

		};
	
}
